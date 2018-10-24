using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace GuiHMM
{
    public partial class Form1 : Form
    {
        int area = -1;
        string iusr = "";
        int Resolution = -1;
        double hmmProb = 0.0;
        double normalStat = 0.0;
        double impProb = 0.0;
        double gangProb = 0.0;
        string today = "";

        List<Panel> listPanel = new List<Panel>();
        [DllImport("HmmDll.dll", CharSet = CharSet.Unicode)]
        public static extern int checkProgrameState();
        [DllImport("HmmDll.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void getLastDate(StringBuilder myString, int length);
        public Form1()
        {
            string date = "xx/xx/xx";
            if (checkProgrameState() == 1)
            {
                StringBuilder str = new StringBuilder(100);
                //get my string from native code
                getLastDate(str, str.Capacity);
                date = str.ToString();
            }
            InitializeComponent();
            label1.Text = "*The data set until " + date;
        }
        //Continu Withot Upload
        private void button2_Click(object sender, EventArgs e)
        {
            if (checkProgrameState() == 0)
            {
                MessageBox.Show("You Must Upload Data", "No Data Eror", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                panel1.Visible = true;
            }
        }
        [DllImport("HmmDll.dll", CharSet = CharSet.Unicode)]
        public static extern int startLoad();
        //upload file
        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Uploading Data may take up to a minute");
            if (startLoad() == 1)
            {
                StringBuilder str = new StringBuilder(100);
                //get my string from native code
                getLastDate(str, str.Capacity);
                label1.Text = "*The data set until " + str.ToString();
                button2.Text = "Continue";
                MessageBox.Show("Data is Set!");
                panel1.Visible = true;
            }
        }
        //get prediction
        [DllImport("HmmDll.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void createSeq(int a, string b);
        [DllImport("HmmDll.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern double getStatPrediction(int startingYear, int area, string iusr, int crimeResolution, int mode, string date);
        [DllImport("HmmDll.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern double getPrediction(int a, string b ,int c);
        private void button3_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedItem != null)
            {
                Object selectedArea = comboBox1.SelectedItem;
                area = Int32.Parse(selectedArea.ToString());
            }
            if (comboBox2.SelectedItem != null)
            {
                Object selectedIusr = comboBox2.SelectedItem;
                iusr = selectedIusr.ToString();
            }
            if (comboBox3.SelectedItem != null)
            {
                Object selectedRes = comboBox3.SelectedItem;
                Resolution = Int32.Parse(selectedRes.ToString());
            }
            if (area != -1 && iusr != "" && Resolution != -1)
            {
                createSeq(area, iusr);
                hmmProb = getPrediction(area, iusr, Resolution);

                StringBuilder str = new StringBuilder(100);
                //get my string from native code
                getLastDate(str, str.Capacity);
                today = str.ToString();
                normalStat = getStatPrediction(2003, area, iusr, Resolution, 0, today);
                impProb = getStatPrediction(2003, area, iusr, Resolution, 1, today);
                gangProb = getStatPrediction(2003, area, iusr, Resolution, 2, today);

                normalStat = normalStat * 100;
                impProb = impProb * 100;
                gangProb = gangProb * 100;
                panel2.Visible = true;
                panel1.Visible = false;

            }
            else
            {
                MessageBox.Show("You must fil all required fields", "Input Eror", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }
        //output - table[IUCR,COMM.AREA,TIME,DATE]
        private void panel2_Paint(object sender, PaintEventArgs e)
        {
            DataTable table = new DataTable();

            table.Columns.Add("HMM", typeof(double));
            table.Columns.Add("Statistical method", typeof(double));
            table.Columns.Add("Improved Prediction", typeof(double));
            table.Columns.Add("Gang Probability", typeof(double));
            table.Rows.Add(hmmProb, normalStat, impProb, gangProb);

            dataGridView1.DataSource = table;
        }
        //back to prediction parameters
        private void button4_Click(object sender, EventArgs e)
        {
            panel2.Visible = false;
            panel1.Visible = true;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            listPanel.Add(panel1);
            listPanel.Add(panel2);
        }
         
        //Show State History
        private void button5_Click(object sender, EventArgs e)
        {
            Process.Start("fsrc\\stateHistory.txt");
        }

        //back from get prediction to main window
        private void button6_Click(object sender, EventArgs e)
        {
            panel1.Visible = false;
        }
    }
}
