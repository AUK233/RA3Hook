using System;
using System.Diagnostics;
using System.IO;
using System.Text;
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
        public byte[] CustomData = Encoding.Unicode.GetBytes("测试测试");

        public MainWindow()
        {
            InitializeComponent();
            textBox.Text = Ra3.GetDefaultRa3Path() ?? string.Empty;
            Task.Run(BackgroundInjectTask);
        }

        private void OnStartGameButtonClicked(object sender, RoutedEventArgs e)
        {
            try
            {
                Process.Start(new ProcessStartInfo
                {
                    FileName = textBox.Text,
                    Arguments = "-ui -win -xres 1840 -yres 1035 -xpos 40 -ypos 85",
                    UseShellExecute = false,
                });
            }
            catch (Exception exception)
            {
                MessageBox.Show($"游戏启动失败：{exception}");
            }
        }

        private void BackgroundInjectTask()
        {
            while (true)
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
                        MessageBox.Show($"Dll 不存在：{DllPath}");
                        continue;
                    }
                    Ra3.InjectAndWaitForExit(process, DllPath, CustomData);
                }
                catch (Exception exception)
                {
                    MessageBox.Show($"注入失败：{exception}");
                }
                finally
                {
                    Task.Delay(1000);
                }
            }
        }
    }
}
