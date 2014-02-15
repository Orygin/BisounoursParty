import std.md5;
import std.stdio;
import std.datetime;
import std.file;
import std.conv;
import std.xml;
import std.getopt;
import std.string;
import std.process;
import std.parallelism;

struct Entry
{
	string name;
	ubyte[16] md5;
}
int ChunkStep = 4096;
void main(string[] args)
{
	if(args.length < 3)
	{
		writeln("Usage : PatchBuilder oldFolder newFolder");
		return;
	}

	string folder1 = args[1];
	string folder2 = args[2];
	string output = args[3];

	if(!folder1.isDir()){
		writeln(folder1, " is not a directory");
		return;
	}
	if(!folder2.isDir()){
		writeln(folder2, " is not a directory");
		return;
	}
	if(!output.isDir()){
		writeln(output, " is not a directory");
		return;
	}

	Entry[] entries1;
	write("Examining file : ");
	foreach (auto name; parallel(dirEntries(folder1, SpanMode.breadth)))
	{
		if(name.isFile())
		{
			entries1 ~= Entry(chompPrefix(name, folder1 ~ "/"),mdFile(name));
			write("\33[2K\r");
			write("Examining file : " ~ name);
		}

	}
	write("\33[2K\r"); // Remove Examining file ...
	writeln(folder1," has ",entries1.length, " entries");
	Entry[] entries2;
	write("Examining file : ");
	foreach (auto name; parallel(dirEntries(folder2, SpanMode.breadth)))
	{
		if(name.isFile())
		{
			entries2 ~= Entry(chompPrefix(name, folder2 ~ "/"),mdFile(name));
			write("\33[2K\r");
                        write("Examining file : " ~ name);
		}
	}
	write("\33[2K\r"); // remove Examining file ...
	writeln(folder2," has ", entries2.length, " entries");

	// Let's compare the folders now
	Entry[] Different;
	Entry[] New;
	Entry[] Deleted;
	int nbrSame = 0;
	foreach(Entry ent; entries1)
	{
		foreach(Entry Comp; entries2)
		{
			if(ent.name == Comp.name)
			{
				if(ent.md5 != Comp.md5)
					Different ~= Comp;
				else
					nbrSame++;
				entries2 = RemoveEntry(entries2, Comp);
				entries1 = RemoveEntry(entries1, ent);
			}
		}
	}
	foreach(Entry ent; entries1)
		Deleted ~= ent;
	foreach(Entry ent; entries2)
		New ~= ent;

	writeln("Found ", Different.length, " different files, ", New.length, " new files, ", Deleted.length, " deleted files and ", nbrSame, " untouched files.");

/*	if(Different.length == 0 && New.length == 0 && Deleted.length == 0)
		return;*/

	writeln("making output folder");

//	if(!isDir("output"))
//		mkdir("output");

	foreach(auto ent; parallel(Different))
	{
		system("cp " ~ folder2 ~ "/" ~ ent.name ~ " " ~ output ~ "/" ~ ent.name);
		//std.file.append(output ~ "/Changelog.txt", "* " ~ ent.name ~"\r\n");
	}
	foreach(auto ent; parallel(New))
	{
		system("cp " ~ folder2 ~ "/" ~ ent.name ~ " " ~ output ~ "/" ~ ent.name);
		//std.file.append(output ~ "/Changelog.txt", "+ " ~ ent.name ~"\r\n");
	}

	auto doc = new Document(new Tag("Files"));
	foreach(Entry e; Deleted){
		//std.file.append(output ~ "/Changelog.txt", "- " ~ e.name ~"\r\n");
		auto el = new Element("File");
		el ~= new Element("name", e.name);
		doc ~= el;
	}
	writeln("writing DeletedFiles document");
	std.file.write(output ~ "/DeletedFiles.xml", doc.toString());

/*	writeln("building patch file");
	system("bin/7z a patch.7z output/ -y");

	writeln("building base folder");
	system("bin/7z a " ~ folder2 ~ ".7z " ~ folder2 ~ "/ -y");
*/
}
Entry[] RemoveEntry(Entry[] list, Entry object)
{
	Entry[] Return;
	bool after = false;
	for(int i =0; i<list.length;i++)
	{
		if(list[i].name == object.name)
			continue;
		Return ~= list[i];
	}
	return Return;
}

/// Digests a file and prints the result.
ubyte[16] mdFile(string filename)
{
	MD5_CTX context;
	ubyte[16] digest;
    context.start();
	File f = File(filename,"r");
    
	foreach (buffer; f.byChunk(ChunkStep))
		context.update(buffer);

    context.finish(digest);
	f.close();
//    writefln("MD5 (%s) = %s", filename, digestToString(digest));
	return digest;
}
