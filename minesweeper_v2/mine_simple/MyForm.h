#pragma once
#include <ctime>	//時間
#include <cstdlib>	//亂數
#include "Form_win.h"

namespace minesimple {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			init_Game();
		}
	protected:
		~MyForm(void)
		{
			if (components)
			{
				delete components;
			}
		}
	
	private:
		#pragma region 變數區
		int ROWS = 9;
		int COLS = 9;
		int num_mines = 10;		//地雷數量
		int Cellsize = 30;		//格子大小
		float Fontsize = 10.0;	//字型大小
		int st_x = 15;
		int st_y = 100;
		int count;		//已經踩過安全區數量
		int mine_flag;	//地雷減標記
		array<array<Button^>^>^ board;  // 遊戲盤面
		array<array<bool>^>^ is_mine;   // 地雷位置
		#pragma endregion
	
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ Mines;
	private: System::ComponentModel::Container^ components;
	private:
		#pragma region 初始化遊戲區域
		void init_Game()
		{
			int weigth = (Cellsize + 1) * COLS + 25;
			int heigth = (Cellsize + 1) * ROWS + (100);
			this->ClientSize = System::Drawing::Size(weigth, heigth); //設定視窗大小
			FlagCount();
			Count();
			board = gcnew array<array<Button^>^>(ROWS);
			is_mine = gcnew array<array<bool>^>(ROWS);
			st_x = 15;

			for (int i = 0; i < ROWS; i++)
			{
				board[i] = gcnew array<Button^>(COLS);
				is_mine[i] = gcnew array<bool>(COLS);
				for (int j = 0; j < COLS; j++)
				{
					board[i][j] = gcnew Button();
					board[i][j]->Location = Point(st_x + j * Cellsize, st_y + i * Cellsize);
					board[i][j]->Size = System::Drawing::Size(Cellsize, Cellsize);
					board[i][j]->Text = "-";
					board[i][j]->Font = (gcnew System::Drawing::Font(L"新細明體", Fontsize, System::Drawing::FontStyle::Bold));
					board[i][j]->Enabled = true;
					board[i][j]->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnButtonClick);
					this->Controls->Add(board[i][j]);
				}
			}

			set_mines(is_mine, num_mines);
		}

		#pragma endregion

	private:
		#pragma region 遊戲結束
		void endGame()
		{
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					if (board[i][j]->Enabled && is_mine[i][j])
					{
						board[i][j]->Text = "X";

					}
					board[i][j]->Enabled = false;
				}
			}
		}
		#pragma endregion

	private:
		#pragma region 生成地雷函式
		void set_mines(array<array<bool>^>^ arr, int mine)
		{
			srand(time(NULL));
			int c = 0;
			while (c < num_mines)
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
	#pragma endregion

	private:
		#pragma region 檢查按鈕
		void Click_button(Button^ button)
		{
			int rows = (button->Location.Y - st_y) / Cellsize;
			int cols = (button->Location.X - st_x) / Cellsize;
			check_mines(button);
			if (button->Text == "X")
			{
				lose();
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

		#pragma endregion

	private:
		#pragma region 檢查是否為地雷 並顯示數字或地雷
		void check_mines(Button^ button)
		{
			//計算按鈕相對位置
			int rows = (button->Location.Y - st_y) / Cellsize;
			int cols = (button->Location.X - st_x) / Cellsize;

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
				else
				{
					button->Text = "";
				}
				if (button->Enabled)
				{
					Count(1);
				}
				button->Enabled = false;
			}
		}
		#pragma endregion

	private:
		#pragma region 按鈕清除
		void clearBoard(array<array<Button^>^>^ Array)
		{
			for each (array<Button^> ^ i in Array)
			{
				for each (Button ^ button in i)
				{
					button->MouseDown -= gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::OnButtonClick);
					this->Controls->Remove(button);
				}
				i->Clear;
			}
			
		}
		#pragma endregion

	public:
		#pragma region 再玩一次;重置遊戲
		void again() {
			clearBoard(board);
			init_Game();
		}
		#pragma endregion

	private:
		#pragma region 數開過的區域數量
		void Count(int i)
		{
			count += i;
			if (count == (COLS * ROWS - num_mines))win();
		}
		void Count()
		{
			Count(-count);
		}
		#pragma endregion

		#pragma region 地雷數 - 標記數
		void FlagCount(int i) 
		{
			mine_flag -= i;
			Mines->Text = mine_flag.ToString();
		}
		void FlagCount()
		{
			mine_flag = num_mines;
			FlagCount(0);
		}
		#pragma endregion

	private:
		#pragma region 呼叫輸贏視窗
		void win() 
		{
			Form_win^ winForm = gcnew Form_win(true);
			set_winForm(winForm);
		}
		void lose() /*踩到地雷結束遊戲*/
		{
			endGame();
			Form_win^ winForm = gcnew Form_win(false);
			set_winForm(winForm);
		}
		#pragma endregion

		#pragma region 設定輸贏視窗
		void set_winForm(Form_win^ winfrm)
		{
			winfrm->again += gcnew tryAgain(this, &MyForm::again);
			winfrm->Show();
		}
		#pragma endregion

		#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->Mines = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(240, 30);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(85, 65);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Reset";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// Mines
			// 
			this->Mines->AutoSize = true;
			this->Mines->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Mines->Location = System::Drawing::Point(52, 48);
			this->Mines->Name = L"Mines";
			this->Mines->Size = System::Drawing::Size(21, 24);
			this->Mines->TabIndex = 1;
			this->Mines->Text = L"0";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(600, 600);
			this->Controls->Add(this->Mines);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"踩地雷遊戲";
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
			if (button->Text == "-")
			{
				button->Text = "!";
				FlagCount(1);
			}
			else if (button->Text == "!")
			{
				button->Text = "-";
				FlagCount(-1);
			}
		}
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		again();
	}

};
}