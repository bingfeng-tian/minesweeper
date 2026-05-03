#pragma once
#include "mainForm.h"

namespace minesweeperv3 {
	
	using namespace System;
	using namespace System::Windows::Forms;

	public ref class square : public System::Windows::Forms::Button
	{
	public:
		square(int size, int fsize, int x, int y)
		{
			this->Height = size;
			this->Width = size;
			this->Font = gcnew System::Drawing::Font(L"¼Ð·¢Åé", fsize, System::Drawing::FontStyle::Bold);
			this->x = x;
			this->y = y;
		}
		
		bool is_mine = false;
		bool work = true;

		int x;
		int y;

		void disable(void) 
		{
			this->Enabled = false;
			work = false;
		}
	};

}
