/**
 * @file Footmap.h
 * @brief 歩数マップを管理するクラス
 */
#ifndef INCLUDED_FOOT_H
#define INCLUDED_FOOT_H

class Footmap{
private:

public:
	uint32_t map[32][32];

	Footmap();
	
	/**
	 * @brief 歩数マップをクリアします
	 */
	void resetFootmap();

	/**
	 * @brief 歩数マップを取得します
	 * @param x 取得するx座標
	 * @param y 取得するy座標
	 * @param out 座標外の返り値
	 * @return 設定した座標の歩数
	 */
	uint16_t getFootmap(int x, int y, int out);

	/**
	 * @brief 歩数マップを設定します
	 * @param x 設定するx座標
	 * @param y 設定するy座標
	 * @param data 設定する歩数
	 */
	bool setFootmap(int x, int y, int data);

	bool isOutside(int x, int  y);

	~Footmap();
	
};

#endif
