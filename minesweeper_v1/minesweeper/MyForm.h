#pragma once
#include <ctime>	//時間
#include <cstdlib>	//亂數

namespace minesweeper {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;

	private:
		int ROWS = 9;
		int COLS = 9;
		int NUM_mines = 10;
		int Cell_size = 30;
		int Font_size = 10;
		int st_x = (400 - (Cell_size + 1) * ROWS) / 2;
		int st_y = 100;
		int count = 0;	//已經踩過安全區數量
		array<array<Button^>^>^ board;  // 遊戲盤面
		array<array<bool>^>^ is_mine;   // 地雷位置
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::NumericUpDown^ numericUpDown1;
	private: System::Windows::Forms::NumericUpDown^ numericUpDown2;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::LinkLabel^ linkLabel1;
	private: System::Windows::Forms::LinkLabel^ linkLabel2;
	private: System::Windows::Forms::LinkLabel^ linkLabel3;
	private: System::Windows::Forms::LinkLabel^ linkLabel4;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;

	public:
		MyForm(void)
		{
			InitializeComponent();
			init_Form();
		}

	private:
		/*初始化視窗*/
		void init_Form()
		{
			int weigth = (COLS < 5) ? 600 : (Cell_size + 1) * COLS + 100;
			int heigth = (ROWS < 5) ? 600 : (Cell_size + 1) * ROWS + 100;
			//this->ClientSize = System::Drawing::Size(weigth, heigth);

			Count();
			textBox1->Text = count.ToString();

			board = gcnew array<array<Button^>^>(ROWS);
			is_mine = gcnew array<array<bool>^>(ROWS);
			st_x = (600 - (Cell_size + 1) * ROWS) / 2;

			// 初始化畫面
			for (int i = 0; i < ROWS; i++)
			{
				board[i] = gcnew array<Button^>(COLS);
				is_mine[i] = gcnew array<bool>(COLS);
				for (int j = 0; j < COLS; j++)
				{
					board[i][j] = gcnew Button();
					board[i][j]->Location = Point(st_x + j * Cell_size, st_y + i * Cell_size);
					board[i][j]->Size = System::Drawing::Size(Cell_size, Cell_size);
					board[i][j]->Font = (gcnew System::Drawing::Font(L"新細明體", Font_size, System::Drawing::FontStyle::Bold));
					board[i][j]->Enabled = true;
					board[i][j]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnButtonClick);
					this->Controls->Add(board[i][j]);
				}
			}

			set_mines(is_mine, NUM_mines);
		}
	private:
		/*更新畫面排版*/
		void updata(int csize, int fsize)
		{
			int weigth = (Cell_size + 1) * COLS + (100);
			int heigth = (Cell_size + 1) * ROWS + (100);
			this->ClientSize = System::Drawing::Size(weigth, heigth); //設定視窗大小
			st_x = (600 - (Cell_size + 1) * ROWS) / 2;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					board[i][j]->Location = Point(st_x + j * csize, st_y + i * csize);
					board[i][j]->Size = System::Drawing::Size(csize, csize);
					board[i][j]->Font = (gcnew System::Drawing::Font(L"新細明體", fsize, System::Drawing::FontStyle::Bold));
				}
			}
		}
	private:
		/*重新遊戲*/
		void reset()
		{
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					board[i][j]->Text = "";
					board[i][j]->Enabled = true;
					board[i][j]->Font = (gcnew System::Drawing::Font(L"新細明體", Font_size, System::Drawing::FontStyle::Bold));
					is_mine[i][j] = false;
				}
			}
			Count();
			set_mines(is_mine, NUM_mines);
		}
	private:
		void endGame()
		{
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					if (board[i][j]->Enabled)
					{
						check_mines(board[i][j]);
					}
				}
			}
		}
	private:
		/*生成地雷*/
		void set_mines(array<array<bool>^>^ arr, int mine)
		{
			srand(time(NULL));
			int c = 0;
			while (c < NUM_mines)
			{
				int x = rand() % ROWS;
				int y = rand() % COLS;
				if (!is_mine[x][y])
				{
					is_mine[x][y] = true;
					c++;
				}
			}
		}
	private:
		void Click_button(Button^ button)
		{
			int rows = (button->Location.Y - st_y) / Cell_size;
			int cols = (button->Location.X - st_x) / Cell_size;
			check_mines(button);
			if (button->Text == "X")
			{
				lose();
			}
			else if (count == (COLS * ROWS - NUM_mines))
			{
				win();
			}
			else if (button->Text == "" && !(button->Enabled))
			{
				for (int i = (rows != 0) ? rows - 1 : rows; i <= rows + 1; i++)
				{
					for (int j = (cols != 0) ? cols - 1 : cols; j <= cols + 1; j++)
					{
						//數九宮格範圍內的地雷
						if (i < ROWS && j < COLS && !(button->Enabled) && button->Text == "")
						{
							check_mines(board[i][j]);
						}
					}
				}
			}
		}
	private:
		void check_mines(Button^ button)
		{
			//按鈕相對位置
			int rows = (button->Location.Y - st_y) / Cell_size;
			int cols = (button->Location.X - st_x) / Cell_size;

			if (is_mine[rows][cols])	//是地雷
			{
				button->Text = "X";
				button->Enabled = false;//設定成不可用
			}
			else	//不是地雷
			{
				int c = 0;
				for (int i = (rows != 0) ? rows - 1 : rows; i <= rows + 1; i++)
				{
					for (int j = (cols != 0) ? cols - 1 : cols; j <= cols + 1; j++)
					{
						//數九宮格範圍內的地雷
						if (i < ROWS && j < COLS && is_mine[i][j])c++;
					}
				}
				if (c != 0)
				{
					button->Text = c.ToString();
				}
				if (button->Enabled)
				{
					Count(1);
				}
				button->Enabled = false;
			}
		}
	private:
		/*刪除按鈕*/
		void clearBoard(array<array<Button^>^>^ Array)
		{
			for each (array<Button^> ^ i in Array)
			{
				for each (Button ^ button in i)
				{
					button->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnButtonClick);
					this->Controls->Remove(button);
				}
			}
		}
	private:
		void win() {
			textBox1->Text = "win";
		}
		void lose() /*踩到地雷結束遊戲*/
		{
			Button^ button;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					button = board[i][j];
					if (is_mine[i][j])
					{
						button->Text = "X";
					}
					button->Enabled = false;	//設定成不可用
				}
			}
			textBox1->Text = "Lose";
		}

	private:
		//注意*地雷不會數到*
		void Count(int i)
		{
			count += i;
		}
		void Count()
		{
			Count(-count);
		}

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code

		/// 
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// 
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->linkLabel2 = (gcnew System::Windows::Forms::LinkLabel());
			this->linkLabel3 = (gcnew System::Windows::Forms::LinkLabel());
			this->linkLabel4 = (gcnew System::Windows::Forms::LinkLabel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(208, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 18);
			this->label1->TabIndex = 1;
			this->label1->Text = L"按鈕大小";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 37);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(80, 18);
			this->label2->TabIndex = 3;
			this->label2->Text = L"地雷數量";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(173, 37);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 18);
			this->label3->TabIndex = 6;
			this->label3->Text = L"區域大小";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(646, 18);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(115, 55);
			this->button1->TabIndex = 7;
			this->button1->Text = L"重新遊戲";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(646, 99);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(115, 55);
			this->button2->TabIndex = 8;
			this->button2->Text = L"結束遊戲";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->Enabled = false;
			this->trackBar1->LargeChange = 1;
			this->trackBar1->Location = System::Drawing::Point(294, 6);
			this->trackBar1->Maximum = 40;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(100, 69);
			this->trackBar1->TabIndex = 11;
			this->trackBar1->TickStyle = System::Windows::Forms::TickStyle::None;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(92, 34);
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(75, 29);
			this->numericUpDown1->TabIndex = 12;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(259, 34);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 30, 0, 0, 0 });
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(75, 29);
			this->numericUpDown2->TabIndex = 13;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 9, 0, 0, 0 });
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown2_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(291, 43);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(26, 18);
			this->label4->TabIndex = 14;
			this->label4->Text = L"小";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(368, 43);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(26, 18);
			this->label5->TabIndex = 15;
			this->label5->Text = L"大";
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(430, 18);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(44, 18);
			this->linkLabel1->TabIndex = 16;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"初級";
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel1_LinkClicked);
			// 
			// linkLabel2
			// 
			this->linkLabel2->AutoSize = true;
			this->linkLabel2->Location = System::Drawing::Point(480, 18);
			this->linkLabel2->Name = L"linkLabel2";
			this->linkLabel2->Size = System::Drawing::Size(44, 18);
			this->linkLabel2->TabIndex = 17;
			this->linkLabel2->TabStop = true;
			this->linkLabel2->Text = L"中級";
			this->linkLabel2->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel2_LinkClicked);
			// 
			// linkLabel3
			// 
			this->linkLabel3->AutoSize = true;
			this->linkLabel3->Location = System::Drawing::Point(530, 18);
			this->linkLabel3->Name = L"linkLabel3";
			this->linkLabel3->Size = System::Drawing::Size(44, 18);
			this->linkLabel3->TabIndex = 18;
			this->linkLabel3->TabStop = true;
			this->linkLabel3->Text = L"高級";
			this->linkLabel3->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel3_LinkClicked);
			// 
			// linkLabel4
			// 
			this->linkLabel4->AutoSize = true;
			this->linkLabel4->Location = System::Drawing::Point(580, 18);
			this->linkLabel4->Name = L"linkLabel4";
			this->linkLabel4->Size = System::Drawing::Size(44, 18);
			this->linkLabel4->TabIndex = 19;
			this->linkLabel4->TabStop = true;
			this->linkLabel4->Text = L"自訂";
			this->linkLabel4->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel4_LinkClicked);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->numericUpDown1);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->numericUpDown2);
			this->groupBox1->Location = System::Drawing::Point(211, 81);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(429, 82);
			this->groupBox1->TabIndex = 20;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Custom";
			this->groupBox1->Visible = false;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(353, 21);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(70, 50);
			this->button3->TabIndex = 21;
			this->button3->Text = L"刷新";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(801, 25);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(115, 55);
			this->button4->TabIndex = 21;
			this->button4->Text = L"TestButton";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(816, 123);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 29);
			this->textBox1->TabIndex = 22;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1000, 600);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->linkLabel4);
			this->Controls->Add(this->linkLabel3);
			this->Controls->Add(this->linkLabel2);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"踩地雷遊戲";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
		/*地雷棋盤 按鈕*/
	private: System::Void OnButtonClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		Button^ button = safe_cast<Button^>(sender);
		if (e->Button == System::Windows::Forms::MouseButtons::Left && button->Text != "!")
		{
			Click_button(button);
		}
		else if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{
			if (button->Text == "")
			{
				button->Text = "!";
			}
			else if (button->Text == "!")
			{
				button->Text = "";
			}
		}
	}
		   /*重置遊戲 按鈕*/
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		reset();
	}
		   /*結束遊戲 按鈕*/
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		endGame();
	}
		   /*按鈕大小 拉桿*/
	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		updata(Cell_size + trackBar1->Value, Font_size + (trackBar1->Value / 4));
	}
		   /*棋盤大小 選數字*/
	private: System::Void numericUpDown2_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		int n = (int)numericUpDown2->Value;
		numericUpDown1->Maximum = (n - 1) * (n - 1) + 1;
	}
		   /*初級*/
	private: System::Void linkLabel1_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		clearBoard(board);
		NUM_mines = 10;
		COLS = 9;
		ROWS = 9;
		init_Form();
	}
		   /*中級*/
	private: System::Void linkLabel2_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		clearBoard(board);
		NUM_mines = 40;
		COLS = 16;
		ROWS = 16;
		init_Form();
	}
		   /*高級*/
	private: System::Void linkLabel3_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		clearBoard(board);
		NUM_mines = 99;
		COLS = 30;
		ROWS = 16;
		init_Form();
	}
		   /*自定*/
	private: System::Void linkLabel4_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		groupBox1->Visible = !groupBox1->Visible;
	}
		   /*刷新棋盤*/
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e)
	{
		clearBoard(board);
		NUM_mines = (int)numericUpDown1->Value;
		ROWS = COLS = (int)numericUpDown2->Value;
		groupBox1->Hide();
		init_Form();
	}
		   /*測試按鈕*/
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e)
	{
		for each (array<Button^> ^ i in board)
		{
			for each (Button ^ j in i)
			{
				j->Text = "";
				j->Enabled = true;
			}
		}
		Count();
	}
	};
}