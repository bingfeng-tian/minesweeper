#pragma once

namespace minesweeperv3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	delegate void tryAgain(); //宣告委派 再玩一次

	public ref class win_lose : public System::Windows::Forms::Form
	{
	public:
		win_lose(void)
		{
			InitializeComponent();
		}
		win_lose(bool is_win)
		{
			InitializeComponent();
			if (is_win)
			{
				label1->Text = "恭喜你贏了";
			}
			else
			{
				label1->Text = "真可惜你輸了";
			}
		}
	protected:
		~win_lose(void)
		{
			if (components)
			{
				delete components;
			}
		}

	public:	event tryAgain^ again;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		   /// 這個方法的內容。
		   /// </summary>
		   void InitializeComponent(void)
		   {
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->button1 = (gcnew System::Windows::Forms::Button());
			   this->button2 = (gcnew System::Windows::Forms::Button());
			   this->label2 = (gcnew System::Windows::Forms::Label());
			   this->SuspendLayout();
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Font = (gcnew System::Drawing::Font(L"新細明體", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(136)));
			   this->label1->Location = System::Drawing::Point(70, 90);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(257, 40);
			   this->label1->TabIndex = 0;
			   this->label1->Text = L"輸贏的提示窗";
			   this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			   // 
			   // button1
			   // 
			   this->button1->Font = (gcnew System::Drawing::Font(L"新細明體", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(136)));
			   this->button1->Location = System::Drawing::Point(269, 195);
			   this->button1->Name = L"button1";
			   this->button1->Size = System::Drawing::Size(97, 37);
			   this->button1->TabIndex = 1;
			   this->button1->Text = L"再玩一次";
			   this->button1->UseVisualStyleBackColor = true;
			   this->button1->Click += gcnew System::EventHandler(this, &win_lose::button1_Click);
			   // 
			   // button2
			   // 
			   this->button2->Font = (gcnew System::Drawing::Font(L"新細明體", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(136)));
			   this->button2->Location = System::Drawing::Point(166, 195);
			   this->button2->Name = L"button2";
			   this->button2->Size = System::Drawing::Size(97, 37);
			   this->button2->TabIndex = 2;
			   this->button2->Text = L"結束遊戲";
			   this->button2->UseVisualStyleBackColor = true;
			   this->button2->Click += gcnew System::EventHandler(this, &win_lose::button2_Click);
			   // 
			   // label2
			   // 
			   this->label2->AutoSize = true;
			   this->label2->Font = (gcnew System::Drawing::Font(L"新細明體", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(136)));
			   this->label2->Location = System::Drawing::Point(188, 9);
			   this->label2->Name = L"label2";
			   this->label2->Size = System::Drawing::Size(178, 18);
			   this->label2->TabIndex = 3;
			   this->label2->Text = L"*關閉視窗以結束遊戲";
			   this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			   // 
			   // Form_win
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(378, 244);
			   this->Controls->Add(this->label2);
			   this->Controls->Add(this->button2);
			   this->Controls->Add(this->button1);
			   this->Controls->Add(this->label1);
			   this->Name = L"Form_win";
			   this->Text = L"提示窗";
			   this->Load += gcnew System::EventHandler(this, &win_lose::Form_win_Load);
			   this->ResumeLayout(false);
			   this->PerformLayout();
		   }
#pragma endregion

		   /*再玩一次按鈕*/
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		again();
		//this->Close();
		//this->Hide();
		this->~win_lose();
	}

		   /*結束遊戲按鈕*/
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}

		   /*載入視窗 設定視窗*/
	private: System::Void Form_win_Load(System::Object^ sender, System::EventArgs^ e) {
		//固定窗口大小
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;	//隱藏最大化按鈕
		this->MinimizeBox = false;	//隱藏最小化按鈕
		this->TopMost = true;		//顯示最上層
		this->ShowInTaskbar = false;//隱藏在工作列表
		this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &win_lose::wForm_Closing);
	}

		   /*視窗關閉 全部關閉*/
	private: System::Void wForm_Closing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		Application::Exit();
	}

	};
}
