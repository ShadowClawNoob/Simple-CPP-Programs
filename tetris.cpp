#include <iostream>
#include <Windows.h>
#include <thread>;
#include <vector>

const int nFieldWidth	{ 12 };
const int nFieldHeight	{ 18 };

const int nScreenWidth	{ 80 };
const int nScreenHeight	{ 30 };

unsigned char* pointerField{ nullptr };

std::wstring tetromino[7]{};

int Rotate(int positionX, int positionY, int rotation) {
	switch (rotation % 4)
	{
	case 0: return positionY * 4 + positionX; // 0 degree
	case 1: return 12 + positionY - (positionX * 4); // 90 degree
	case 2: return 15 - (positionY * 4) - positionX; // 180 degree
	case 3: return 3 - positionY + (positionX * 4); // 270 degree
	}

	return 0;
}

bool doesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY) {
	for (int px = 0; px < 4; px++) {
		for (int py = 0; py < 4; py++) {

			int pi{ Rotate(px, py, nRotation) };
			int fi{ (nPosY + py) * nFieldWidth + (nPosX + px) };

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth &&
				nPosY + py >= 0 && nPosY + py < nFieldHeight &&
				tetromino[nTetromino][pi] == L'X' && pointerField[fi] != 0) {

				return false;
			}
		}
	}

	return true;
}

void CreateShape(int index,
	std::wstring row1,
	std::wstring row2,
	std::wstring row3,
	std::wstring row4
) {
	tetromino[index].append(row1);
	tetromino[index].append(row2);
	tetromino[index].append(row3);
	tetromino[index].append(row4);
}


void CreateAssets() {
	CreateShape(0,
		L"..X.",
		L"..X.",
		L"..X.",
		L"..X.");

	CreateShape(1,
		L"..X.",
		L".XX.",
		L".X..",
		L"....");

	CreateShape(2,
		L".X..",
		L".XX.",
		L"..X.",
		L"....");

	CreateShape(3,
		L"....",
		L".XX.",
		L".XX.",
		L"....");


	CreateShape(4,
		L"..X.",
		L".XX.",
		L"..X.",
		L"....");

	CreateShape(5,
		L"....",
		L".XX.",
		L"..X.",
		L"..X.");

	CreateShape(6,
		L"....",
		L".XX.",
		L".X..",
		L".X..");
}


int main()
{
	// Windows Console & Game Logic Setup
	CreateAssets();

	pointerField = new unsigned char[nFieldWidth * nFieldHeight];
	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			pointerField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
		}
	}

	wchar_t* screen{ new wchar_t[nScreenWidth * nScreenHeight] };
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
		screen[i] = L' ';
	}

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten{ 0 };
	
	// Game Logic

	bool gameOver		{ false };
	bool canRotate		{ false };

	int nCurrentPiece	{ 1 };
	int nCurrentRotation{ 0 };
	int nCurrentX		{ nFieldWidth / 2 };
	int nCurrentY		{ 0 };

	bool bkey[4]		{};

	const int N_SPEED	{ 20 };
	int N_SPEED_COUNTER	{ 0 };
	bool bForceDown		{ false };

	int nScore			{ 0 };

	std::vector<int> vLines{};

	while (!gameOver) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		N_SPEED_COUNTER++;
		bForceDown = (N_SPEED_COUNTER == N_SPEED);

		// User Input
		for (int k = 0; k < 4; k++) {
			bkey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
		}

		if (bkey[1] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) {
			nCurrentX = nCurrentX - 1;
		}
		if (bkey[0] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) {
			nCurrentX = nCurrentX + 1;
		} 
		if (bkey[2] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
			nCurrentY = nCurrentY + 1;
		} 
		if (bkey[3]) {
			nCurrentRotation += (!canRotate && doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			canRotate = true;
		}
		else {
			canRotate = false;
		}

		// Game Render Loop
		if (bForceDown) {
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++;
			}
			else {
				// Lock the current piece in the field
				for (int px = 0; px < 4; px++) {
					for (int py = 0; py < 4; py++) {
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X') {
							pointerField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1; 
						}
					}
				}
				
				// Check Pieces lining up horizontally
				for (int py = 0; py < 4; py++) {
					if (nCurrentY + py < nFieldHeight - 1) {
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++) {
							bLine &= (pointerField[(nCurrentY + py) * nFieldWidth + px]) != 0;
						}

						if (bLine) {
							for (int px = 1; px < nFieldWidth - 1; px++) {
								pointerField[(nCurrentY + py) * nFieldWidth + px] = 8;
							}

							vLines.push_back(nCurrentY + py);
						}
					}
				}

				nScore += 25;
				if (!vLines.empty()) {
					nScore += (1 << vLines.size()) * 100;
				}

				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				gameOver = !doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}

			N_SPEED_COUNTER = 0;
		}
		

		// Draw Field
		for (int x = 0; x < nFieldWidth; x++) {
			for (int y = 0; y < nFieldHeight; y++) {
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pointerField[y * nFieldWidth + x]];
			}
		}

		// Draw Current Place
		for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X') {
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; //>>: represents A letts from ASCII
				}
			}
		}

		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

		if (!vLines.empty()) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(400));

			for (auto &v : vLines) {
				for (int px = 1; px < nFieldWidth - 1; px++) {
					for (int py = v; py > 0; py--) {
						pointerField[py * nFieldWidth + px] = pointerField[(py - 1) * nFieldWidth + px];
					}
					pointerField[0 * nFieldWidth + px] = 0;
				}
			}

			vLines.clear();
		}
	}

	CloseHandle(hConsole);
	std::cout << "Game Over! Score:" << nScore << '\n';
	system("pause");

	return 0;
}
