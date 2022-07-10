using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace LuaExecutor
{
    public partial class UnnamedExecutor : Form
    {

        private NamedPipeServerStream server;
        private StreamWriter writer;

        private bool running = false;
        private string clientname = "ACLuaClient.dll";

        [DllImport("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;
        public static string base64Encode(string plainText)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);
            return System.Convert.ToBase64String(plainTextBytes);
        }

        public UnnamedExecutor()
        {
            InitializeComponent();
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void attachButton_Click(object sender, EventArgs e)
        {
            if (server != null)
                server.Dispose();

            server = new NamedPipeServerStream("LuaExecutor", PipeDirection.Out, 1, PipeTransmissionMode.Byte);
            server.WaitForConnection();

            writer = new StreamWriter(server);
            running = true;
        }

        private void executeButton_Click(object sender, EventArgs e)
        {
            if (!running) return;
            string encodedScript = base64Encode(scriptBox.Text);
            writer.Write(encodedScript);
            writer.Write((char)0);
            writer.Flush();
            server.WaitForPipeDrain();
        }

        private void Topbar_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
            }
        }
    }
}
