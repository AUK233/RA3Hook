using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
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

namespace RA3Configurator
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        // ini
        public string SettingPath;
        public string configPath;
        public MainWindow()
        {
            InitializeComponent();
            string LanucherPath = AppDomain.CurrentDomain.BaseDirectory;
            SettingPath = System.IO.Path.Combine(LanucherPath, "2Setting.ini");
            configPath = System.IO.Path.Combine(LanucherPath, "mapping\\config.ini");

            string INIPath = SettingPath;
            if (File.Exists(configPath))
            {
                INIPath = configPath;
            }

            if (INIfile.ReadINTData("RandomCrate", INIPath) > 0)
            {
                CheckBoxCrate.IsChecked = true;
            }

            if (INIfile.ReadINTData("PowerfulMode", INIPath) > 0)
            {
                CheckBoxPower.IsChecked = true;
            }

            if (INIfile.ReadINTData("EnhancedMap", INIPath) > 0)
            {
                CheckBoxMap.IsChecked = true;
            }
        }

        private void OnRandomCratesChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxCrate.IsChecked == true)
            {
                INIfile.WriteData("RandomCrate", "1", SettingPath, configPath);
            }
            else
            {
                INIfile.WriteData("RandomCrate", "0", SettingPath, configPath);
            }
        }
        private void OnPowerfulModeChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxPower.IsChecked == true)
            {
                INIfile.WriteData("PowerfulMode", "1", SettingPath, configPath);
            }
            else
            {
                INIfile.WriteData("PowerfulMode", "0", SettingPath, configPath);
            }
        }
        private void OnEnhancedMapChanged(object sender, RoutedEventArgs e)
        {
            if (CheckBoxMap.IsChecked == true)
            {
                INIfile.WriteData("EnhancedMap", "1", SettingPath, configPath);
            }
            else
            {
                INIfile.WriteData("EnhancedMap", "0", SettingPath, configPath);
            }
        }
    }

    internal static class INIfile
    {
        public static string ReadString(string section, string key, string def, string filePath)
        {
            StringBuilder str = new StringBuilder(1024);
            GetPrivateProfileString(section, key, def, str, 1024, filePath);
            return str.ToString();
        }
        public static uint ReadINT(string section, string key, int value, string filePath)
        {
            return GetPrivateProfileInt(section, key, value, filePath);
        }
        public static uint ReadINTData(string key, string filePath)
        {
            return GetPrivateProfileInt("SkirmishSetting", key, 0, filePath);
        }

        public static int Write(string section, string key, string value, string filePath)
        {
            return WritePrivateProfileString(section, key, value, filePath);
        }

        public static void WriteData(string key, string value, string SettingPath, string configPath)
        {
            WritePrivateProfileString("SkirmishSetting", key, value, SettingPath);
            if (File.Exists(configPath))
            {
                WritePrivateProfileString("SkirmishSetting", key, value, configPath);
            }
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern uint GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefault, string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern int WritePrivateProfileString(string lpApplicationName, string lpKeyName, string lpString, string lpFileName);
    }
}
