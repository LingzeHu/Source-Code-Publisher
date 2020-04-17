///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Pr3                                  //
// ver 1.0                                                           //
// Lingze Hu,   CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Pr3 demo.  
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translator.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 13 Apr 2018
 * - first release
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Windows.Input;
using System.Threading;
using MsgPassingCommunication;



namespace GUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string toPath = "", fromPath = "", path, pattern, regex;
        ITranslator translator = TranslatorFactory.createTranslator();

        public MainWindow()
        {
            InitializeComponent();
        }

        private Stack<string> pathStack_= new Stack<string>();
        private Stack<string> toPathStack_ = new Stack<string>();
        private Stack<string> fromPathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        //private string currentDirType = "";
        private int toPort, fromPort;

        private string getPort(string address)
        {
            string res = "";
            if(address.Contains("localhost:"))
            {
                int pos = address.IndexOf("localhost:");
                res = address.Substring(pos + 10);
            }
            return res;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //Console.Write("Demonstrating Project #4 - Remote Code Page Management \n  ");

            String[] arguments = Environment.GetCommandLineArgs();
            fromPath = arguments[1];
            toPath = arguments[2];
            fromPort = Int32.Parse(getPort(fromPath));
            toPort = Int32.Parse(getPort(toPath));

            // start Comm
            endPoint_ = new CsEndPoint();
            //endPoint_.machineAddress = "localhost";
            //endPoint_.port = 8082;

            endPoint_.machineAddress = "localhost";
            endPoint_.port = fromPort;

            translater = new Translater();
            translater.listen(endPoint_);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            //serverEndPoint.port = 8080;
            serverEndPoint.port = toPort;
            pathStack_.Push(path);
            
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            
        }

        // publish button click
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            //serverEndPoint.port = 8080;
            serverEndPoint.port = toPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "publishFiles");
            msg.add("toPath", toPath);
            msg.add("fromPath", fromPath);
            msg.add("pattern", pattern);
            msg.add("regex", regex);
            translater.postMessage(msg);

            Console.Write("\n  ");
        }

        //----< load getDirs processing into dispatcher dictionary >-------

        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load passFile processing into dispatcher dictionary >------

        private void DispatcherLoadPassFile()
        {
            Action<CsMessage> passFile = (CsMessage rcvMsg) =>
            {                
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile2CovertedFileList(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("passFile", passFile);
        }

        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadPassFile();
        }

        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        private void clearDirs()
        {
            DirList.Items.Clear();
        }

        private void addDir(string dir)
        {
            DirList.Items.Add(dir);
        }

        private void insertParent()
        {
            DirList.Items.Insert(0, "..");
        }

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }


        private void PatternInfo_TextChanged(object sender, TextChangedEventArgs e)
        {
            //pattern = patternInfo.Text;
        }

        private void RegexInfo_TextChanged(object sender, TextChangedEventArgs e)
        {
            //regex = regexInfo.Text;
        }

        private void FromPathInfo_TextChanged(object sender, TextChangedEventArgs e)
        {
            //fromPath = fromPathInfo.Text;
        }

        //from_nagivate button click
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            //currentDirType = "fromPath_";
            toPathLabel.Visibility = Visibility.Hidden;
            fromPathLabel.Visibility = Visibility.Visible;
            DispatcherLoadGetDirs();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            //serverEndPoint.port = 8080;
            serverEndPoint.port = toPort;

            fromPathStack_.Push("..");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", fromPathStack_.Peek());
            translater.postMessage(msg);

        }

        //to_nagivate button click
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            //currentDirType = "toPath_";
            toPathLabel.Visibility = Visibility.Visible;
            fromPathLabel.Visibility = Visibility.Hidden;
            DispatcherLoadGetDirs();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            //serverEndPoint.port = 8080;
            serverEndPoint.port = toPort;

            toPathStack_.Push("..");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", toPathStack_.Peek());
            translater.postMessage(msg);
        }

        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)DirList.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == "..")
            {
                    if (pathStack_.Count > 1)  // don't pop off "Storage"
                        pathStack_.Pop();
                    else
                        return;
            }
            else
            {
                    path = pathStack_.Peek() + "/" + selectedDir;
                    pathStack_.Push(path);
            }
            // display path in TextBlcok
            //fromPathInfo.Text = removeFirstDir(fromPathStack_.Peek());
            pathInfo.Text = pathStack_.Peek();

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            //serverEndPoint.port = 8080;
            serverEndPoint.port = toPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);

            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);

        }

        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        private void CovertedFileList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            String fileName = FileList.SelectedItem.ToString();
            String filePath = "../ConvertedWebpages/" + fileName + ".html";
            //String filePath = toPath + "/" + fileName + ".html";
            String fullFilePath = translator.getFullFileSpec(filePath);
            Console.Write("\n Opening" + fullFilePath);
            //System.Diagnostics.Process.Start("pause");
            System.Diagnostics.Process.Start(fullFilePath);
            Console.Write("\n=======================================================\n");
            Console.Write("Req 7,8 completed.\n");
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void CovertedFileList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //String fileName = FileList.SelectedItem.ToString();
            //String filePath = "../../../ConvertedWebpages/" + fileName + ".html";
            ////String filePath = toPath + "/" + fileName + ".html";
            //String fullFilePath = translator.getFullFileSpec(filePath);
            //System.Diagnostics.Process.Start(fullFilePath);
        }

        private void ToPathInfo_TextChanged(object sender, TextChangedEventArgs e)
        {
            //toPath = toPathInfo.Text;
        }

        private void PathInfo_TextChanged(object sender, TextChangedEventArgs e)
        {
            path = pathInfo.Text;
        }

        private void addFile(string file)
        {
            FileList.Items.Add(file);
        }

        private void addFile2CovertedFileList(string file)
        {
            CovertedFileList.Items.Add(file);
        }

        private void FileList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if(FileList.Items.Count >0)
            {
                String fileName = FileList.SelectedItem.ToString();
                String filePath = path + "/" + fileName;


                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                //serverEndPoint.port = 8080;
                serverEndPoint.port = toPort;


                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "passFile");
                msg.add("path", filePath);
                translater.postMessage(msg);
            }
            


        }

        private void clearFiles()
        {
            FileList.Items.Clear();
        }
    }


}
