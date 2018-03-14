// Vincent Ha
// April 24, 2017
// Lab 1

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
using namespace std;

static const int maxBitSize = 8;

// File_Contents reads and stores the information from a binary file to a string called data
class File_Contents {

private:
	ifstream fin;
	string data;

public:
	File_Contents() {};
	void readFile(string fn, string fp);
	string getData();
};

// CharToAscii converts each character found in the binary file to a series of 2 bit bitsets that can be easily converted to ascii
class CharToBitset {
public:
	string data;
	vector <bitset <2>> bitData;

public:
	CharToBitset();
	void setData(string d);
	void toBitset();
	vector <bitset <2>> getBitData();
};

// BitsetToAscii translates the 2 bit bitsets to larger bitsets that correspond to the key specified in the problem description
class BitsetToAscii {
private:
	vector <char> cipher;
	vector <bitset <2>> bitData;
	string bitsetInString;       // Used to store bitsets in string form. Used in the decodeCharacter function
	string message;
	void decodeCharacter(bitset <2> c);

public:
	BitsetToAscii();
	void setBitData(vector <bitset<2>> bD);
	void decodeMessage();
	string getMessage();
};

// File_Decoder handles all the reading and decrypting, using the objects previously defined
class File_Decoder {
private:
	File_Contents* reader;
	CharToBitset* characterConverter;
	BitsetToAscii* bitsetConverter;
public:
	File_Decoder();
	~File_Decoder();
	void readData();
	void translateData();
	string getMessage();
};

// UserInterface handles the "menu" that the user sees in the console
class UserInterface {
private:
	File_Decoder* decoder;

public:
	UserInterface();
	~UserInterface();
	void menu();
};

// Main Function
int _tmain(int argc, _TCHAR* argv[])
{
	UserInterface uI;
	uI.menu();
	return 0;
}

// Function Defintions
// File_Contents functions
void File_Contents::readFile(string fn, string fp) {
	string content;
	fin.open(fp, ios::in | ios::binary);

	if (!fin.is_open()) {
		cout << "Invalid File Path. Please Reload with Correct File Path" << endl;
		exit(-92);
	}
	cout << "Extracting Data From " + fn + "." << endl;

	int size;
	char* contents;

	fin.seekg(0, ios::end);
	size = fin.tellg();
	contents = new char[size];
	fin.seekg(0, ios::beg);
	fin.read(contents, size);
	fin.close();

	string entireFile(contents);
	delete[] contents;
	data = entireFile;
}

string File_Contents::getData() {
	return data;
}

// CharToBitset Functions
CharToBitset::CharToBitset() : data("N/A") {}

void CharToBitset::setData(string d) {
	data = d;
}

void CharToBitset::toBitset() {
	bitset <8> mask;
	for (int count1 = 0; count1 < data.size(); count1++) {
		bitset<8> temp(data[count1]);
		for (int count2 = 0; count2 < temp.size(); count2 += 2) {
			mask.reset();
			mask.set(maxBitSize - count2 - 1);
			mask.set(maxBitSize - count2 - 2);
			bitset <8> tempInput = (temp & mask) >> (maxBitSize - count2 - 2);
			bitset <2> input(tempInput.to_ulong());
			bitData.push_back(input);
		}
	}
}

vector <bitset <2> > CharToBitset::getBitData() {
	return bitData;
}

// BitsetToAscii Functions
void BitsetToAscii::decodeCharacter(bitset <2> c) {
	if (c.to_string() != "11" && c.to_string() != "00")
		bitsetInString += c.to_string();

	if (c.to_string() == "00") {
		bitset<16> character(bitsetInString);
		message += cipher[character.to_ulong()];
		bitsetInString.clear();
	}

	if (c.to_string() == "11") {
		bitset<16> character(bitsetInString);
		message += cipher[character.to_ulong()];
		bitsetInString.clear();
		message += " ";
	}
}

BitsetToAscii::BitsetToAscii() : bitData() {
	cipher.resize(10858);
	// basic key: . = 10  - = 01          Ascii  Morse	    Binary     Decimal
	cipher[9] = 'A';					  // A    .-         1001         9
	cipher[106] = 'B';					  // B   -...      01101010      106
	cipher[102] = 'C';					  // C   -.-.      01100110      102
	cipher[26] = 'D';					  // D   -..        011010        26
	cipher[2] = 'E';					  // E     .	      10          2
	cipher[166] = 'F';					  // F   ..-.      10100110		 166
	cipher[22] = 'G';					  // G    --.		010110		  22
	cipher[170] = 'H';					  // H   ....	   10101010		 170
	cipher[10] = 'I';					  // I    ..		 1010		  10
	cipher[149] = 'J';					  // J   .---	   10010101		 149
	cipher[25] = 'K';					  // K    -.-		011001		  25
	cipher[154] = 'L';					  // L   .-..	   10011010		 154
	cipher[5] = 'M';					  // M    --		 0101		  5
	cipher[6] = 'N';					  // N    -.		 0110		  6
	cipher[21] = 'O';					  // O    ---       010101        21
	cipher[150] = 'P';					  // P   .--.      10010110		 150
	cipher[89] = 'Q';					  // Q   --.-      01011001		  89
	cipher[38] = 'R';					  // R    .-.       100110		  38
	cipher[42] = 'S';					  // S    ...		101010		  42
	cipher[1] = 'T';					  // T    -			  01		   1
	cipher[41] = 'U';					  // U    ..-		101001		  41
	cipher[169] = 'V';					  // V   ...-      10101001		 169
	cipher[37] = 'W';					  // W    .--       100101		  37
	cipher[105] = 'X';					  // X   -..-      01101001		 105
	cipher[101] = 'Y';					  // Y   -.--      01100101      101
	cipher[90] = 'Z';					  // Z   --..	   01011010		  90
	cipher[341] = '0';					  // 0	 -----    0101010101     341
	cipher[597] = '1';					  // 1   .----	  1001010101	 597
	cipher[661] = '2';					  // 2   ..---    1010010101     661
	cipher[677] = '3';					  // 3   ...--	  1010100101     677
	cipher[681] = '4';					  // 4   ....-    1010101001     681
	cipher[682] = '5';					  // 5   .....    1010101010     682
	cipher[426] = '6';					  // 6   -....    0110101010     426
	cipher[362] = '7';					  // 7   --...    0101101010     362
	cipher[346] = '8';					  // 8   ---..    0101011010     346
	cipher[342] = '9';					  // 9   ----.    0101010110     342
	cipher[2390] = '\'';				  // '   .----.  100101010110	2390
	cipher[2406] = '@';					  // @   .--.-.  100101100110   2406   
	cipher[1386] = ':';					  // :   ---...  010101101010	1386
	cipher[1445] = ',';					  // ,   --..--  010110100101   1445
	cipher[10857] = '$';				  // $   ...-..- 10101001101001 10857
	cipher[425] = '=';					  // =   -...-    0110101001     425
	cipher[1637] = '!';					  // !   -.-.--  011001100101   1637
	cipher[2457] = '.';					  // .   .-.-.-  100110011001   2457
	cipher[2650] = '?';					  // ?   ..--..  101001011010	2650
	cipher[2470] = '"';					  // "   .-..-.  100110100110   2470
}

void BitsetToAscii::setBitData(vector <bitset <2>> bD) {
	bitData = bD;
}

void BitsetToAscii::decodeMessage() {
	for (int count = 0; count < bitData.size(); count++) {
		decodeCharacter(bitData[count]);
	}

	bitsetInString.clear();
}

string BitsetToAscii::getMessage() {
	return message;
}

// File_Decoder functions
File_Decoder::File_Decoder() {
	reader = new File_Contents;
	characterConverter = new CharToBitset;
	bitsetConverter = new BitsetToAscii;
}

File_Decoder::~File_Decoder() {
	delete reader;
	delete characterConverter;
	delete bitsetConverter;
}

void File_Decoder::readData() {
	reader->readFile("Morse.bin", "c:\\Users\\Vincent Ha\\Downloads\\Morse.bin");
	characterConverter->setData(reader->getData());
}

void File_Decoder::translateData() {
	characterConverter->toBitset();
	bitsetConverter->setBitData(characterConverter->getBitData());
	bitsetConverter->decodeMessage();
}

string File_Decoder::getMessage() {
	return bitsetConverter->getMessage();	
}

// UserInterface Functions
UserInterface::UserInterface() {
	decoder = new File_Decoder;
}

UserInterface::~UserInterface() {
	delete decoder;
}

void UserInterface::menu() {
	bool onceMore = true;
	string choice = "something";

	cout << "Welcome to the Morse Code Translation Service" << endl;
	cout << "---------------------------------------------" << endl;
	cout << endl;
	cout << "Progress" << endl;
	cout << "--------" << endl;
	decoder->readData();
	cout << "Translating Data" << endl;
	decoder->translateData();
	cout << "Translation Complete" << endl;
	cout << endl;
	cout << "Message:" << endl;
	cout << "--------" << endl;
	cout << decoder->getMessage() << endl;
	cout << endl;
	cout << "Good Bye!" << endl;
}