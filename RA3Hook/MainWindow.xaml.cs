using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Shapes;

namespace RA3Hook
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string LanucherPath = AppDomain.CurrentDomain.BaseDirectory;
        public string DllPath;
        //public byte[] CustomData = Encoding.Unicode.GetBytes("Test test");
        // The first 4-bytes is the type of read configuration
        public byte[] CustomData = {0, 0, 0, 0, 0, 0, 0, 0};

        // ini
        public string INIPath;
        //
        public string BattleNetPath = "";

        public MainWindow()
        {
            InitializeComponent();
            DllPath = System.IO.Path.Combine(LanucherPath, "DllCore.dll");
            INIPath = System.IO.Path.Combine(LanucherPath, "1Setting.ini");
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
            if (INIfile.ReadINT("CFALauncher", "DirectLaunch", 0, INIPath) > 0)
            {
                CheckBoxStart.IsChecked = true;
            }
            //
            if (INIfile.ReadINT("CFALauncher", "BallteNetRun", 0, INIPath) > 0)
            {
                CheckBattleNet.IsChecked = true;
            }
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
                string gameEXE;
                string runArg = "-config \"";
                if (CheckBoxStart.IsChecked == true)
                {
                    runArg += System.IO.Path.Combine(textBox.Text, $"RA3_{Ra3.GetRa3Language()}_1.12.skudef");
                    runArg += "\" ";
                    gameEXE = System.IO.Path.Combine(textBox.Text, "Data\\ra3_1.12.game");
                    runArg += extraArg;
                }
                else
                {
                    gameEXE = System.IO.Path.Combine(textBox.Text, "RA3.exe");
                    runArg = extraArg;
                }
                //
                Process.Start(new ProcessStartInfo
                {
                    FileName = gameEXE,
                    Arguments = runArg,
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
            if (CheckBattleNet.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "BallteNetRun", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "BallteNetRun", "0", INIPath);
            }
            INIfile.Write("CFALauncher", "BallteNetPath", BNBox.Text, INIPath);
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

        private void OnCheckedBNChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBattleNet.IsChecked == true)
            {
                if (BattleNetPath == "")
                {
                    string BNPath = INIfile.ReadString("CFALauncher", "BallteNetPath", "", INIPath);
                    if (BNPath == "")
                    {
                        BNBox.Text = Ra3.GetDefaultBattleNetPath() ?? string.Empty;
                    }
                    else
                    {
                        BNBox.Text = BNPath;
                    }
                }
                BattleNetPath = BNBox.Text;
            }
            else
            {
                BattleNetPath = "";
            }
        }

        private void OnBattleNetPathChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBattleNet.IsChecked == true)
            {
                BattleNetPath = BNBox.Text;
            }
        }

        private void OnCheckedCPUChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxCPU.IsChecked == true)
            {
                CustomData[4] = 1;
            }
            else
            {
                CustomData[4] = 0;
            }
        }

        private void OnCheckedBloomChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxBloom.IsChecked == true)
            {
                CustomData[5] = 1;
            }
            else
            {
                CustomData[5] = 0;
            }
        }
        private void OnCheckedDebugChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxDebug.IsChecked == true)
            {
                CustomData[6] = 1;
            }
            else
            {
                CustomData[6] = 0;
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
                    
                    tude1 = Ra3.InjectAndWaitForExit(process, DllPath, CustomData, 5000);
                    if (tude1 && BattleNetPath != "")
                    {
                        if (File.Exists(BattleNetPath))
                        {
                            byte[] BNLog = Encoding.UTF8.GetBytes(LanucherPath+"\\logs");
                            Ra3.InjectAndWaitForExit(process, BattleNetPath, BNLog, 100);
                        }
                        else
                        {
                            MessageBox.Show("Invalid BattleNet Path!");
                        }
                    }
                }
                catch (Exception exception)
                {
                    MessageBox.Show($"Injection Failure: {exception}");
                    button.Content = "Restart";
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
