namespace Updater
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.m_buUpdate = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.m_lLocalVersion = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.m_lNewVersion = new System.Windows.Forms.Label();
            this.logBox = new System.Windows.Forms.RichTextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.MirrorBox = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.GeneralProgressBar = new System.Windows.Forms.ProgressBar();
            this.label3 = new System.Windows.Forms.Label();
            this.PatchProgressBar = new System.Windows.Forms.ProgressBar();
            this.LogButton = new System.Windows.Forms.Button();
            this.ToolTips = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // m_buUpdate
            // 
            this.m_buUpdate.Location = new System.Drawing.Point(339, 4);
            this.m_buUpdate.Name = "m_buUpdate";
            this.m_buUpdate.Size = new System.Drawing.Size(75, 23);
            this.m_buUpdate.TabIndex = 0;
            this.m_buUpdate.Text = "Update";
            this.m_buUpdate.UseVisualStyleBackColor = true;
            this.m_buUpdate.Click += new System.EventHandler(this.m_buUpdate_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Local version :";
            // 
            // m_lLocalVersion
            // 
            this.m_lLocalVersion.AutoSize = true;
            this.m_lLocalVersion.Location = new System.Drawing.Point(94, 9);
            this.m_lLocalVersion.Name = "m_lLocalVersion";
            this.m_lLocalVersion.Size = new System.Drawing.Size(13, 13);
            this.m_lLocalVersion.TabIndex = 2;
            this.m_lLocalVersion.Text = "?";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(200, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Latest version :";
            // 
            // m_lNewVersion
            // 
            this.m_lNewVersion.AutoSize = true;
            this.m_lNewVersion.Location = new System.Drawing.Point(280, 9);
            this.m_lNewVersion.Name = "m_lNewVersion";
            this.m_lNewVersion.Size = new System.Drawing.Size(13, 13);
            this.m_lNewVersion.TabIndex = 4;
            this.m_lNewVersion.Text = "?";
            // 
            // logBox
            // 
            this.logBox.Location = new System.Drawing.Point(15, 90);
            this.logBox.Name = "logBox";
            this.logBox.ReadOnly = true;
            this.logBox.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.logBox.Size = new System.Drawing.Size(479, 181);
            this.logBox.TabIndex = 5;
            this.logBox.Text = "";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(416, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "Exit";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // MirrorBox
            // 
            this.MirrorBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.MirrorBox.FormattingEnabled = true;
            this.MirrorBox.Location = new System.Drawing.Point(246, 29);
            this.MirrorBox.Name = "MirrorBox";
            this.MirrorBox.Size = new System.Drawing.Size(245, 21);
            this.MirrorBox.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(200, 32);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Mirror :";
            // 
            // GeneralProgressBar
            // 
            this.GeneralProgressBar.Location = new System.Drawing.Point(15, 73);
            this.GeneralProgressBar.Name = "GeneralProgressBar";
            this.GeneralProgressBar.Size = new System.Drawing.Size(479, 11);
            this.GeneralProgressBar.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 37);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Progress :";
            // 
            // PatchProgressBar
            // 
            this.PatchProgressBar.Location = new System.Drawing.Point(15, 56);
            this.PatchProgressBar.Name = "PatchProgressBar";
            this.PatchProgressBar.Size = new System.Drawing.Size(479, 11);
            this.PatchProgressBar.TabIndex = 13;
            // 
            // LogButton
            // 
            this.LogButton.Image = global::Updater.Properties.Resources.log;
            this.LogButton.Location = new System.Drawing.Point(317, 4);
            this.LogButton.Name = "LogButton";
            this.LogButton.Size = new System.Drawing.Size(20, 23);
            this.LogButton.TabIndex = 14;
            this.LogButton.UseVisualStyleBackColor = true;
            this.LogButton.Click += new System.EventHandler(this.LogButton_Click);
            // 
            // ToolTips
            // 
            this.ToolTips.Tag = "Saucisse";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(506, 283);
            this.Controls.Add(this.LogButton);
            this.Controls.Add(this.PatchProgressBar);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.GeneralProgressBar);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.MirrorBox);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.logBox);
            this.Controls.Add(this.m_lNewVersion);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.m_lLocalVersion);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_buUpdate);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Bisounours Party Updater";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_buUpdate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label m_lLocalVersion;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label m_lNewVersion;
        private System.Windows.Forms.RichTextBox logBox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox MirrorBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ProgressBar GeneralProgressBar;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ProgressBar PatchProgressBar;
        private System.Windows.Forms.Button LogButton;
        private System.Windows.Forms.ToolTip ToolTips;
    }
}

