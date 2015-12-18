/**
 * @file Walldata.h
 */
#ifndef INCLUDED_WALLDATA_H
#define INCLUDED_WALLDATA_H

#include "const.h"

/**
 * @brief 壁データを簡単に扱うためのクラス。<br>
 * MapクラスやSensorクラスとの壁情報のやりとりに用いる方が良い。
 */
class Walldata{
private:
	char data;

public:
	/**
	 * @brief コンストラクタ<br>初期値は壁なし<br>右ビットから順に前，左，右，後のデータが格納されている
	 */
	Walldata();

	/**
	 * @brief =演算子のオーバーロード<br>char型の代入ができます。
	 * @param input 代入したい変数
	 */
	void operator= (char input);

	void operator= (Walldata input);

	void operator+= (char input);

	void operator|= (char input);

	/**
	 * @brief クラスが保持している内部データを返します。
	 * @return クラスが保持している内部データ
	 */
	char getRawData();

	/**
	 * @brief 指定した方向に壁があるか返します。
	 * @param dir 壁を調べたい方向
	 * @return 壁があれば1，なければ0をintで返します
	 */
	int existWall(EMouseDirection dir);

	/**
	 * @brief 指定した方向に壁を追加します。
	 * @param dir 壁を追加したい方向
	 */
	void addWall(EMouseDirection dir);

	/**
	 * @brief 指定した方向の壁を除去します。
	 * @param angle 壁を除去したい方向
	 */
	void removeWall(EMouseDirection angle);

	~Walldata();
};

#endif
