#!/usr/bin/env python
import shutil
import os
import time
import datetime
import math
import urllib
import urllib2
import sys
import pylzma
import xml.etree.ElementTree as ET
from py7zlib import Archive7z, NoPasswordGivenError, WrongPasswordError

def _iter_python26(node):
	return [node] + node.findall('.//*')
  
def ensure_dir(f):
	d = os.path.dirname(f)
	if not os.path.exists(d):
		os.makedirs(d)

Debug = False
StartScript = None
#Bump this number if you insist on using an older updater version
CurrentUpdaterVersion = 14

for i, arg in enumerate(sys.argv):
	if arg == "-start":
		print "Start parameter found. Starting script when updated"
		StartScript = sys.argv[i+1]
	if arg == "-h":
		print "This script will update your local copy of bisounours party."
		print "You can make this script automatically run the server by adding"
		print " -start /path/to/end/script.sh"
		print "This will skip any wait and run your script that launch the server"
		print "Debug output can be activated using -d"
		sys.exit()
	if arg == "-d":
		print "Debug output activated"
		Debug = True

print "Checking local version of game"
LocalFile = open('version', 'r')
LocalVersion = LocalFile.readline()
Channel = LocalFile.readline()
print 'Local version is ' + LocalVersion
if(Channel != ''):
	print 'Channel is ' + Channel
LocalFile.close()

Host = "http://www.bisounoursparty.com/NebuleuseDev/server_i486.so" if (Channel.strip() == "dev") else "http://www.bisounoursparty.com/nebuleuse/server_i486.so";

print "Checking last version of game"
Host = "http://www.bisounoursparty.com/NebuleuseDev/UpdateInfo.php" if (Channel.strip() == "dev") else "http://www.bisounoursparty.com/nebuleuse/UpdateInfo.php";
filehandle = urllib.urlopen(Host)
LineVersions = filehandle.readline()
Versions = LineVersions.split(' ', 1)
UpdaterVersion = int(Versions[0])
LastVersion = Versions[1]

if(UpdaterVersion > CurrentUpdaterVersion):
	print "There is a new updater version available. Download it at http://www.bisounoursparty.com/nebuleuse/update.py"
	sys.exit()

Mirror = filehandle.readline().split(' ', 1)[0]

print "Last version of the game is " + LastVersion
Iterations = int(LastVersion) - int(LocalVersion)

if(Iterations <= 0):
	print "No new update needed."

	if not os.exists("server_i486"):
		print "Server binary not found, downloading"
		BinaryUrl = urllib2.urlopen(Host)
		BinaryFile = open('server_i486.so','wb')
		BinaryFile.write(BinaryUrl.read())
		BinaryFile.close()

	if StartScript is not None:
		os.system("bash " + StartScript)
	sys.exit()

print "You need to do " + str(Iterations) + " updates"

if StartScript is None:
	raw_input("Press Enter to continue...")
	
i = 0

while(i < Iterations):
	i += 1
	PatchNbr = str(int(LocalVersion) + i)
	print "Downloading patch #" + PatchNbr
	UpdateFile = urllib2.urlopen(Mirror+PatchNbr+".update")
	output = open('../updates/'+PatchNbr+'.patch','wb')
	output.write(UpdateFile.read())
	output.close()

	# Download the file and store it
	fp = file('../updates/'+PatchNbr+'.patch','rb')
	print "Downloaded patch, Extracting"

	archive = Archive7z(fp)
	files = archive.getmembers()
	for f in files: 
	# Open the archive and extract the DeletedFiles manifest
		if(f.filename.strip() == "bpr/DeletedFiles.xml"):
			outfile = open('../../'+f.filename, 'wb')
			outfile.write(f.read())
			outfile.close()
			break
	# Don't close archive yet, we still need to extract the whole update
	
	#parse the DeletedFiles manifest and remove unused files
	tree = ET.parse('../DeletedFiles.xml')
	root = tree.getroot()
	for subfile in root:
		fname = subfile.find('name').text
		try:
			os.remove('../' + fname)
		except:
			if Debug:
				print 'File couldn\'t be removed: ' + fname
		else:
			if Debug:
				print 'Removed ' + fname

	for i, f in enumerate(files):
	# Open the archive and extract its files
		if Debug:
			print 'Processing :' + f.filename
			
		ensure_dir('../../'+f.filename)
		outfile = open('../../'+f.filename, 'wb')
		outfile.write(f.read())
		outfile.close()
		# Display progress
		b = ("Progress : " + str(i+1) + "/" + str(len(files)))
		sys.stdout.write('\r'+b)
		sys.stdout.flush()
		
	print "\rDone"
		
	fp.close()
	print "Cleaning up Patch file"
	os.remove('../updates/'+PatchNbr+'.patch')
	
print "Downloading latest binary"

BinaryUrl = urllib2.urlopen(Host)
BinaryFile = open('server_i486.so','wb')
BinaryFile.write(BinaryUrl.read())
BinaryFile.close()
	
print "You now have the latest version of the game"
# We don't want to launch a script, quit now
if StartScript is None:
	sys.exit()

print "Running end script"
os.system("bash " + StartScript)
