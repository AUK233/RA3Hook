using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace RA3Hook
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string DllPath = Path.GetFullPath("DllCore.dll");
        //public byte[] CustomData = Encoding.Unicode.GetBytes("Test test");
        public byte[] CustomData = {0,0,0,0};

        // ini
        public string INIPath = INIfile.GetINIPath();

        public MainWindow()
        {
            InitializeComponent();
            //
            string gamePath = INIfile.ReadString("CFALauncher", "GamePath", "", INIPath);
            if (gamePath == "")
            {
                textBox.Text = Ra3.GetDefaultRa3Path() ?? string.Empty;
            }
            else
            {
                textBox.Text = gamePath;
            }
            //
            string modPath = INIfile.ReadString("CFALauncher", "modPath", "", INIPath);
            if (modPath == "")
            {
                textBoxMF.Text = Ra3.GetModPath() ?? string.Empty;
            }
            else
            {
                textBoxMF.Text = modPath;
            }
            // here!!!!!!!!!!!
            textBoxMN.Text = INIfile.ReadString("CFALauncher", "modName", "CFA_2.300", INIPath);
            //
            textBoxArg.Text = INIfile.ReadString("CFALauncher", "GameEXArg", "", INIPath);
            //
            if (INIfile.ReadINT("CFALauncher", "isWin", 0, INIPath) > 0)
            {
                CheckBoxWin.IsChecked = true;
            }
            //
            if (INIfile.ReadINT("CFALauncher", "SetCPU", 0, INIPath) > 0)
            {
                CheckBoxCPU.IsChecked = true;
            }
            //
            if (INIfile.ReadINT("CFALauncher", "NoBloom", 0, INIPath) > 0)
            {
                CheckBoxBloom.IsChecked = true;
            }
            //
            if (INIfile.ReadINT("CFALauncher", "SetDebug", 0, INIPath) > 0)
            {
                CheckBoxDebug.IsChecked = true;
            }
            //
            Task.Run(BackgroundInjectTask);
        }

        private void OnStartGameButtonClicked(object sender, RoutedEventArgs e)
        {
            try
            {
                string extraArg = "-modConfig \"";
                extraArg += textBoxMF.Text;
                extraArg += "\\";
                extraArg += textBoxMN.Text;
                extraArg += ".skudef\"";
                //
                if (CheckBoxWin.IsChecked == true)
                {
                    extraArg += " -win";
                }

                if (textBoxArg.Text != "")
                {
                    extraArg += " ";
                    extraArg += textBoxArg.Text;
                }
                //extraArg += " -xres 1840 -yres 1035 -xpos 40 -ypos 85";
                Process.Start(new ProcessStartInfo
                {
                    FileName = textBox.Text,
                    Arguments = extraArg,
                    UseShellExecute = false,
                });
                button.Content = "Running!";
            }
            catch (Exception exception)
            {
                MessageBox.Show($"Start Game Failure: {exception}");
            }
        }

        private void OnSaveSettingButtonClicked(object sender, RoutedEventArgs e)
        {
            SaveINISetting();
            System.Media.SystemSounds.Asterisk.Play();
        }

        private void SaveINISetting()
        {
            INIfile.Write("CFALauncher", "GamePath", textBox.Text, INIPath);
            INIfile.Write("CFALauncher", "modPath", textBoxMF.Text, INIPath);
            INIfile.Write("CFALauncher", "modName", textBoxMN.Text, INIPath);
            INIfile.Write("CFALauncher", "GameEXArg", textBoxArg.Text, INIPath);
            //
            if (CheckBoxWin.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "isWin", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "isWin", "0", INIPath);
            }
            //
            if (CheckBoxCPU.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "SetCPU", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "SetCPU", "0", INIPath);
            }
            //
            if (CheckBoxBloom.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "NoBloom", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "NoBloom", "0", INIPath);
            }
            //
            if (CheckBoxDebug.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "SetDebug", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "SetDebug", "0", INIPath);
            }
        }

        private void OnCheckedCPUChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxCPU.IsChecked == true)
            {
                CustomData[0] = 1;
            }
            else
            {
                CustomData[0] = 0;
            }
        }

        private void OnCheckedBloomChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxBloom.IsChecked == true)
            {
                CustomData[1] = 1;
            }
            else
            {
                CustomData[1] = 0;
            }
        }
        private void OnCheckedDebugChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxDebug.IsChecked == true)
            {
                CustomData[2] = 1;
            }
            else
            {
                CustomData[2] = 0;
            }
        }

        private void BackgroundInjectTask()
        {
            bool tude1 = false;
            while (!tude1)
            {
                try
                {
                    var window = Ra3.FindRa3Window();
                    if (window == IntPtr.Zero)
                    {
                        continue;
                    }
                    var process = Ra3.FindRa3Process(window);
                    if (process == null)
                    {
                        continue;
                    }
                    if (!File.Exists(DllPath))
                    {
                        MessageBox.Show($"Missing Dll: {DllPath}");
                        continue;
                    }
                    
                    tude1 = Ra3.InjectAndWaitForExit(process, DllPath, CustomData);
                }
                catch (Exception exception)
                {
                    MessageBox.Show($"Injection Failure: {exception}");
                }
                finally
                {
                    Task.Delay(1000);
                }
            }

            // Close launcher window
            System.Environment.Exit(0);
        }
    }
}
