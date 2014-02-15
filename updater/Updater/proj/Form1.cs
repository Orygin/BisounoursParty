using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Threading;
using System.Diagnostics;
using System.Xml;
using Updater.Properties;

namespace Updater
{
    public partial class Form1 : Form
    {
        const int UpdaterVersion = 13;

#region Var declarations
        bool updateNeeded;
        bool debug;

        string WorkDir;
        string Channel = "release";
#region Delegates
        delegate void addnewlogline(string text, bool debug = false);
        addnewlogline oAffA;

        delegate void addNewMirror(MirrorBoxItem item);
        addNewMirror oAddMirr;

        delegate void EndUpdate();
        EndUpdate endedup;
        EndUpdate dSetUpdateNeeded;

        delegate void UpdateServerText(string text);
        UpdateServerText oUpServTxt;

        delegate void GeneralUpProgressBar(int progress);
        GeneralUpProgressBar oGeneralUpProgress;
        delegate void PatchUpProgressBar(int progress);
        PatchUpProgressBar oPatchUpProgress;
#endregion
        int iterations;
        int ServerVersion;
        int LocalVersion;
#endregion
        // Create the updater stuff
        public Form1(string[] args)
        {
            InitializeComponent();
            Height = 114;
            m_buUpdate.Enabled = false;
            ToolTips.SetToolTip(m_buUpdate, "Start updating");
            ToolTips.SetToolTip(LogButton, "Show log");

            oAffA = new addnewlogline(addLogLine);
            endedup = new EndUpdate(Finishdupdates);
            oAddMirr = new addNewMirror(addMirror);
            oUpServTxt = new UpdateServerText(updateServText);
            oGeneralUpProgress = new GeneralUpProgressBar(UpdateGeneralProgressBar);
            oPatchUpProgress = new PatchUpProgressBar(UpdatePatchProgressBar);
            dSetUpdateNeeded = new EndUpdate(SetUpdateNeeded);

            debug = false;
            updateNeeded = false;

            ShowInTaskbar = true;

            WorkDir = Directory.GetCurrentDirectory() + "\\";
#region Start args
            if (args.Length != 0) // Process args
            {
                int i = 0;
                foreach (string arg in args)
                {
                    if (arg == "-debug")
                        debug = true;

                    else if (arg == "-path")
                    {
                        WorkDir = args[i+1];
                    }
                    i++;
                }
            }
#endregion
            if (debug)
            {
                addLogLine("Updater started in debug mode", true);
                addLogLine("Working dir is : " + WorkDir, true);
            }
#region Check for self update
            //First check if we are a new updater
            if (System.AppDomain.CurrentDomain.FriendlyName == "newUpdater.exe")
            {
                if (debug)
                {
                    addLogLine("We are a new updater, replace the older one", true);
                }

                Process[] processes = Process.GetProcessesByName("Updater.exe");
                if (processes.Length > 0)
                {
                    processes[0].CloseMainWindow(); // Close the old updater that may have stayed too long
                    processes[0].Kill();
                }

                File.Delete("Updater.exe");
                File.Copy("newUpdater.exe", "Updater.exe");

                ProcessStartInfo p = new ProcessStartInfo();
                p.FileName = "Updater.exe";
                p.WorkingDirectory = WorkDir;
                Process x = Process.Start(p);

                System.Environment.Exit(0); // destroy us
                return;
            }
            else if (File.Exists("newUpdater.exe"))
            {
                Process[] processes = Process.GetProcessesByName("newUpdater.exe");
                if (processes.Length > 0)
                {
                    processes[0].CloseMainWindow(); // Close the old updater that may have stayed too long
                    processes[0].Kill();
                }
                File.Delete("newUpdater.exe");
                if (debug)
                {
                    addLogLine("We are not new updater, but it exist. Deleted", true);
                }
            }
#endregion
#region Exctract 7z
            string exePath = WorkDir + "\\7z.exe";
            File.WriteAllBytes(exePath, Resources._7zr);

            if (!File.Exists(exePath))
            {
                addLogLine("Couldn't extract 7z. This application may be corrupted.");
                m_buUpdate.Enabled = false;
                return;
            }
#endregion
            addLogLine("Updater version " + UpdaterVersion);
#region Load current version
            string path = WorkDir + "\\" + "version";

            if (!File.Exists(path))
            {
                MessageBox.Show("Local Version not found\n Reinstalling the game may correct this problem", "Critical Error");
                addLogLine("Local Version not found\n Reinstalling the game may correct this problem");
                m_buUpdate.Enabled = false;
                return;
            }
            string[] versionfile = File.ReadAllLines(path);
            if (versionfile.Length > 1)
            {
                addLogLine("Channel : " + versionfile[1]);
                Channel = versionfile[1];
            }
            addLogLine("Current version is " + versionfile[0]);
            m_lLocalVersion.Text = versionfile[0];
            LocalVersion = Convert.ToInt32(versionfile[0]);
#endregion
            addLogLine("Fetching server informations");

            Activate();

            Thread t = new Thread(() => GetServerInfo());
            t.Start();
        }
        // Get the server infos, parse it and prepare to update
        void GetServerInfo()
        {
            string gameVersion = "";
            string updaterVersion = "";
            StreamReader sr;
            WebResponse Response;
            #region Get server infos
            try
            {
                string server = Channel == "dev" ? "http://www.bisounoursparty.com/NebuleuseDev/UpdateInfo.php" : "http://www.bisounoursparty.com/nebuleuse/UpdateInfo.php";
                HttpWebRequest myRequest = (HttpWebRequest)WebRequest.Create(server);
                myRequest.Method = "GET";
                myRequest.Proxy = null;
                Response = myRequest.GetResponse();
                sr = new StreamReader(Response.GetResponseStream(), System.Text.Encoding.UTF8);
            }
            catch (WebException e)
            {
                Invoke(oAffA, new object[] { "Couldn't get server informations", false });
                Invoke(oAffA, new object[] { e.ToString(), true });
                m_buUpdate.Enabled = false;
                return;
            }
            #endregion
            #region Parse that shit'
            try
            {
                char[] c = new char[] { ' ' };

                string result = sr.ReadLine(); // The first line is the updater version then game version
                string[] versions = result.Split(c, 2);
                updaterVersion = versions[0];
                gameVersion = versions[1];

                string mirror = sr.ReadLine(); // the next lines are mirror sites

                while (mirror != null)
                {
                    if (debug)
                        Invoke(oAffA, new object[] { "Mirrors infos : " + mirror, true });

                    string[] s = mirror.Split(c, 2);
                    MirrorBoxItem item = new MirrorBoxItem();
                    item.Value = s[0];
                    item.Text = s[1];
                    Invoke(oAddMirr, new object[] { item });
                    mirror = sr.ReadLine();
                }
            }
            catch (Exception e)
            {
                Invoke(oAffA, new object[] { "Couldn't parse server informations", false });
                return;
            }
            finally
            {
                sr.Close();
                Response.Close();
            }
            #endregion
            #region Updater needs updating
            if (Convert.ToInt32(updaterVersion) > UpdaterVersion)
            { // There is an updater update available. Download it under a different name
                Invoke(oAffA, new object[] { "New updater version available. Downloading.", false });
                try
                {
                    using (WebClient Client = new WebClient())
                    {
                        Client.DownloadFile("http://www.bisounoursparty.com/nebuleuse/Updater.exe", "newUpdater.exe");
                    }
                }
                catch (Exception e)
                {
                    Invoke(oAffA, new object[] { "Could not download the latest updater.\n Get it from : \n http://www.bisounoursparty.com/nebuleuse/Updater.exe", false });
                    updateNeeded = false;
                    m_buUpdate.Enabled = false;
                    return;
                }
                MessageBox.Show("There is a new version of the updater ready. The updater will restart now to apply the new version.", "Attention", MessageBoxButtons.OK);

                ProcessStartInfo p = new ProcessStartInfo();
                p.FileName = "newUpdater.exe";
                p.WorkingDirectory = WorkDir;
                p.WindowStyle = ProcessWindowStyle.Hidden;
                Process x = Process.Start(p);

                System.Environment.Exit(0); // destroy us
            }
            #endregion
            #region Update gui and prepare to update if any
            Invoke(oAffA, new object[] { "Lastest version of the game is " + gameVersion, false });

            ServerVersion = Convert.ToInt32(gameVersion);
            iterations = ServerVersion - LocalVersion;

            Invoke(oUpServTxt, new object[] { gameVersion });

            if (iterations < 1)
            {
                Invoke(oAffA, new object[] { "No new update needed", false });
                updateNeeded = false;
            }
            else
            {
                Invoke(oAffA, new object[] { iterations + " new updates available", false });
                updateNeeded = true;
                Invoke(dSetUpdateNeeded);
            }
#endregion
        }        
        // Update in succession to the latest version
        void RetrieveAndUpdate(string mirror)
        {
            Invoke(oAffA, new object[] { "Starting the update", false });

            for (int i = 0; i < iterations; i++)
            {
#region Download patch i+1
                Invoke(oPatchUpProgress, new object[] { 0 });
                int currentpatch = (i+1) + Convert.ToInt32(LocalVersion);
                Invoke(oAffA, new object[] { "Downloading patch #" + currentpatch.ToString(), false });

                string url = mirror + currentpatch.ToString() + ".update";
                string dest = WorkDir + "../updates/" + currentpatch.ToString() + ".patch";
                string relDest = "../updates/" + currentpatch.ToString() + ".patch"; // 7z doesn't like absolute path
                WebClient client = new WebClient();
                try
                {
                    // We are forced to use async to use the progress callbacks
                    // But since we are already on a seperate thread, just wait for it to finish
                    client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(client_DownloadProgressChanged);
                    client.Proxy = null;
                    client.DownloadFileAsync(new Uri(url), dest);
                    while (client.IsBusy) { ;; }

                    Invoke(oAffA, new object[] { "Finished Downloading patch #" + currentpatch.ToString(), false });
                }
                catch(WebException e)
                {
                    Invoke(oAffA, new object[] { "Error, patch #" + currentpatch.ToString() + " not found !!", false });
                    Invoke(oAffA, new object[] { e.ToString(), false });
                    return;
                }
#endregion
#region Extract DeletedFiles.xml
                ProcessStartInfo p;
                Process x;
                try
                {
                    p = new ProcessStartInfo(); // Extracting the deleted files info
                    p.FileName = "7z.exe";
                    p.Arguments = "x " + relDest + " bpr/DeletedFiles.xml -o..\\..\\ -aoa -y";
                    p.WorkingDirectory = WorkDir;
                    p.WindowStyle = ProcessWindowStyle.Hidden;
                    if (debug)
                    {
                        addLogLine("Decompressing deletedfiles.xml", true);
                        p.RedirectStandardOutput = true;
                        p.UseShellExecute = false; // Uncomment to debug
                    }
                    x = Process.Start(p);

                    if(debug)
                        Invoke(oAffA, new object[] { x.StandardOutput.ReadToEnd(), true});

                    x.WaitForExit();
                }
                catch (Exception e)
                {
                    Invoke(oAffA, new object[] { "There was an error extracting with 7z", false });
                    Invoke(oAffA, new object[] { e.ToString(), false });
                }
#endregion
#region Delete unused files
                try
                {
                    if (File.Exists(@"..\DeletedFiles.xml"))
                    {
                        using (XmlReader reader = XmlReader.Create(new StringReader(File.ReadAllText(@"../DeletedFiles.xml", Encoding.ASCII))))
                        {
                            while (reader.ReadToFollowing("name"))
                            {
                                string DelFile = reader.ReadElementContentAsString();

                                if (File.Exists(@"..\" + DelFile))
                                    File.Delete(@"..\" + DelFile);
                            }
                        }
                    }
                }
                catch (Exception)
                {
                    Invoke(oAffA, new object[] { "There was an error deleting unused files", false });
                }
#endregion
#region Extract full patch
                try
                {
                    p = new ProcessStartInfo(); // Extracting the full patch
                    p.FileName = "7z.exe";
                    p.Arguments = "x " + relDest + " -o..\\..\\ -aoa -y";
                    p.WorkingDirectory = WorkDir;
                    p.WindowStyle = ProcessWindowStyle.Hidden; 

                    if (debug)
                    {
                        addLogLine("Decompressing patch "+ dest, true);
                        p.RedirectStandardOutput = true;
                        p.UseShellExecute = false;
                    }

                    x = Process.Start(p);

                    if(debug)
                        Invoke(oAffA, new object[] { x.StandardOutput.ReadToEnd(), true });

                    x.WaitForExit();
                }
                catch (Exception e)
                {
                    Invoke(oAffA, new object[] { "There was an error extracting with 7z", false });
                    Invoke(oAffA, new object[] { e.ToString(), false });
                }
#endregion
                Invoke(oAffA, new object[] { "Patch #" + currentpatch.ToString() + " Applied", false });
#region cleanup
                try
                {
                    File.Delete(dest);
                }
                catch (FileNotFoundException) { } //We don't really care if the file we wanted to delete doesn't exist even tho that's troubling
#endregion
                Invoke(oGeneralUpProgress, new object[] { 100 / iterations * (i+1)}); // Update progress bar
            }
            Invoke(oGeneralUpProgress, new object[] { 100 }); // Update progress bar
            Invoke(endedup);
        }
        // Check for problems (current version isn't latest version ?) and open changelog
        void Finishdupdates()
        {
            #region Re-check version to see if we updated correctly
            string path = WorkDir + "\\" + "version";

            if (!File.Exists(path))
                addLogLine("Local version file not found after updating\n Reinstalling the game may correct this problem");

            string[] versionfile = File.ReadAllLines(path);
            if (versionfile.Length > 1)
            {
                addLogLine("Channel : " + versionfile[1]);
                Channel = versionfile[1];
            }
            addLogLine("Current version is " + versionfile[0]);
            m_lLocalVersion.Text = versionfile[0];
            if (LocalVersion == Convert.ToInt32(versionfile[0])) // We are still on old version
                addLogLine("Version number is the same as before. Patching failed");
            else if (ServerVersion != Convert.ToInt32(versionfile[0]))
                addLogLine("Version number is not the same as the expected server version. Patching failed");
            else
                LocalVersion = Convert.ToInt32(versionfile[0]);
            #endregion
            #region Open changelog
            DialogResult res = MessageBox.Show("Your game is now up to date. Open changelog file ?", "Done", MessageBoxButtons.YesNo);
            if (res.ToString() == "Yes")
            {
                ProcessStartInfo n = new ProcessStartInfo();
                n.FileName = "notepad";
                n.Arguments = @"..\CHANGELOG.TXT";
                n.WorkingDirectory = Directory.GetCurrentDirectory();
                Process x = Process.Start(n);
            }
            #endregion
        }

        //Static stuff
        private void Form1_Load(object sender, EventArgs e)
        {
            Activate();
        }
        public void updateServText(string txt)
        {
            m_lNewVersion.Text = txt;
        }
        private void m_buUpdate_Click(object sender, EventArgs e)
        {
            if (updateNeeded && MirrorBox.Items.Count > 0)
            {
                string sr = ((MirrorBoxItem)MirrorBox.SelectedItem).Value;
                Thread t = new Thread(() => RetrieveAndUpdate(sr));
                t.Start();
                m_buUpdate.Enabled = false;
            }
        }
        private void UpdateGeneralProgressBar(int progress)
        {
            GeneralProgressBar.Value = progress;
        }
        private void UpdatePatchProgressBar(int progress)
        {
            PatchProgressBar.Value = progress;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
        private void SetUpdateNeeded()
        {
            m_buUpdate.Enabled = true;
        }

        void client_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            double bytesIn = double.Parse(e.BytesReceived.ToString());
            double totalBytes = double.Parse(e.TotalBytesToReceive.ToString());
            double percentage = bytesIn / totalBytes * 100;
            Invoke(oPatchUpProgress, new object[] { int.Parse(Math.Truncate(percentage).ToString()) });
        }

        public void addLogLine(string text, bool debug = false)
        {
            if (debug)
                logBox.Text += "Debug : " + text + "\n";
            else
                logBox.Text += text + "\n";

            logBox.SelectionStart = logBox.Text.Length;
            logBox.ScrollToCaret();

            File.AppendAllText("log.txt", text + "\r\n");
        }
        public void addMirror(MirrorBoxItem item)
        {
            MirrorBox.Items.Add(item);
            MirrorBox.SelectedIndex = 0;
        }

        private void LogButton_Click(object sender, EventArgs e)
        {
            Height = Height == 114 ? 309 : 114;
        }
    }

    public class MirrorBoxItem
    {
        public string Text { get; set; }
        public string Value { get; set; }

        public override string ToString()
        {
            return Text;
        }
    }
}
