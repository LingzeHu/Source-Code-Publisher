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
using System.Windows.Shapes;
using System.IO;
using System.Windows.Markup;
using System.Windows.Forms;

namespace GUI
{
    /// <summary>
    /// Interaction logic for CodePopUpWin.xaml
    /// </summary>
    public partial class CodePopUpWin : Window
    {
        MainWindow mWin = null;
        ITranslator translator;

        public CodePopUpWin()
        {
            InitializeComponent();
        }


        public void setMainWindow(MainWindow mwin)
        {
            mWin = mwin;
        }
        
        public void setNameLabel(String fileName)
        {
            nameLabel.Text = fileName;
        }

        public void displayFile(String fileName)
        {
            translator = TranslatorFactory.createTranslator();
            String filePath = "../ConvertedWebpages/" + fileName + ".html";
            String fullFilePath = translator.getFullFileSpec(filePath);

            Uri uri = new Uri(fullFilePath);
            myWebBrowser.Navigate(uri);
            
            //myWebBrowser.ScriptErrorsSuppressed = true;

            System.Diagnostics.Process.Start(filePath);
            //setNameLabel(fileName);
            //FileStream textFile = new FileStream(filePath, FileMode.Open, FileAccess.Read);
            //codeView = XamlReader.Load(textFile) as FlowDocument;
            ////flowDocRdr.Document = content;
            //textFile.Close();

        }

    }
}
