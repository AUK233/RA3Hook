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
        public byte[] CustomData = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        // ini
        public string INIPath;
        //
        public string BattleNetPath = "";
        //
        public bool backgroundInject = true;

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
            if (INIfile.ReadINT("CFALauncher", "ForceLocal", 0, INIPath) > 0)
            {
                CheckBoxLocal.IsChecked = true;
            }
            //
            Task.Run(BackgroundInjectTask);
        }

        private void OnStartGameButtonClicked(object sender, RoutedEventArgs e)
        {
            try
            {
                int runType = 0;
                if (CheckBoxStart.IsChecked == true)
                {
                    runType = 1;
                }
                LanuchGame(runType);
            }
            catch (Exception exception)
            {
                MessageBox.Show($"Start Game Failure: {exception}");
            }
        }
        
        private void OnPowerStartGameButtonClicked(object sender, RoutedEventArgs e)
        {
            try
            {
                LanuchGame(2);
            }
            catch (Exception exception)
            {
                MessageBox.Show($"Start Game Failure: {exception}");
            }
        }

        private void LanuchGame(int runType)
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
            //
            if (runType > 0)
            {
                runArg += System.IO.Path.Combine(textBox.Text, $"RA3_{Ra3.GetRa3Language()}_1.12.skudef");
                runArg += "\" ";
                gameEXE = System.IO.Path.Combine(textBox.Text, "Data\\ra3_1.12.game");
                runArg += extraArg;
                // turn off background injection
                backgroundInject = false;
                // initialization parameters
                string BNPath = "";
                if (CheckBattleNet.IsChecked == true)
                {
                    BNPath = BNBox.Text;
                }
                int returnValue;
                if (runType == 2)
                {
                    string configurePath = System.IO.Path.Combine(LanucherPath, "mapping\\config.ini");
                    SaveDllConfigure(configurePath);
                    // run!
                    returnValue = Ra3.PowerfulRA3Launch(gameEXE, runArg, BNPath);
                }
                else
                {
                    button.Content = "Running!";
                    //System.Media.SystemSounds.Beep.Play();
                    byte[] BNLog = Encoding.UTF8.GetBytes(LanucherPath + "\\logs");
                    returnValue = Ra3.LanuchAndInject(gameEXE, runArg, DllPath, CustomData, BNPath, BNLog);
                }
                //
                if(returnValue == 0)
                {
                    System.Environment.Exit(0);
                }
                else
                {
                    backgroundInject = true;
                    button.Content = "Restart";
                }
            }
            else
            {
                gameEXE = System.IO.Path.Combine(textBox.Text, "RA3.exe");
                runArg = extraArg;
                Process.Start(new ProcessStartInfo
                {
                    FileName = gameEXE,
                    Arguments = runArg,
                    UseShellExecute = false,
                });
                button.Content = "Running!";
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
            //
            if (CheckBoxLocal.IsChecked == true)
            {
                INIfile.Write("CFALauncher", "ForceLocal", "1", INIPath);
            }
            else
            {
                INIfile.Write("CFALauncher", "ForceLocal", "0", INIPath);
            }
        }
        private void SaveDllConfigure(string path)
        {
            if (CheckBoxCPU.IsChecked == true)
            {
                INIfile.Write("CFASetting", "SetCPU", "1", path);
            }
            else
            {
                INIfile.Write("CFASetting", "SetCPU", "0", path);
            }
            //
            if (CheckBoxBloom.IsChecked == true)
            {
                INIfile.Write("CFASetting", "NoBloom", "1", path);
            }
            else
            {
                INIfile.Write("CFASetting", "NoBloom", "0", path);
            }
            //
            if (CheckBoxDebug.IsChecked == true)
            {
                INIfile.Write("CFASetting", "SetDebug", "1", path);
            }
            else
            {
                INIfile.Write("CFASetting", "SetDebug", "0", path);
            }
            //
            if (CheckU60FPS.IsChecked == true && CheckBattleNet.IsChecked == false)
            {
                INIfile.Write("CFASetting", "DualFPS", "1", path);
            }
            else
            {
                INIfile.Write("CFASetting", "DualFPS", "0", path);
            }
            //
            if (CheckBoxLocal.IsChecked == true)
            {
                INIfile.Write("CFASetting", "ForceLocal", "1", path);
            }
            else
            {
                INIfile.Write("CFASetting", "ForceLocal", "0", path);
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
        private void OnChecked60FPSChanged(object sender, RoutedEventArgs e)
        {
            if (CheckU60FPS.IsChecked == true)
            {
                if (CheckBattleNet.IsChecked == false)
                {
                    CustomData[7] = 1;
                }
            }
            else
            {
                CustomData[7] = 0;
            }
        }
        private void OnCheckedLocalSaveChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxLocal.IsChecked == true)
            {
                CustomData[8] = 1;
            }
            else
            {
                CustomData[8] = 0;
            }
        }

        private void BackgroundInjectTask()
        {
            bool tude1 = false;
            while (!tude1)
            {
                try
                {
                    if(backgroundInject)
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
                            //continue;
                        }

                        tude1 = Ra3.InjectAndWaitForExit(process, DllPath, CustomData, 5000);
                        if (tude1 && BattleNetPath != "")
                        {
                            if (File.Exists(BattleNetPath))
                            {
                                byte[] BNLog = Encoding.UTF8.GetBytes(LanucherPath + "\\logs");
                                Ra3.InjectAndWaitForExit(process, BattleNetPath, BNLog, 100);
                            }
                            else
                            {
                                MessageBox.Show("Invalid BattleNet Path!");
                            }
                        }
                    }
                }
                catch (Exception exception)
                {
                    MessageBox.Show($"Injection Failure: {exception}");
                }
                finally
                {
                    //Task.Delay(1000);
                    Thread.Sleep(50);
                }
            }

            // Close launcher window
            System.Environment.Exit(0);
        }
    }
}
