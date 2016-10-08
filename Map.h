/*
 * @file Map.h
 */
#pragma once

#include "SlalomParams.h"
#include "Walldata.h"

class Map{
private:
	
	
public:
	Map();

	/**
	 * @brief 一番左の壁がMSB，右から2番目の壁がLSB，下から順
	 */
	uint32_t column[32];
	/**
	 * @brief 一番左の壁がMSB，右から2番目の壁がLSB，下から順
	 */
	uint32_t row[32];
	/**
	 * @brief 一番左のマスがMSB，一番右のマスがLSB，下から順
	 */
	uint32_t reached[32];


	void format();
	/**
	 * @brief 迷路データを初期化する
	 */
	void formatWall();
	/**
	 * @brief 到達マップを初期化する
	 */
	void formatReached();


	/**
	 * @brief 壁を追加します
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 * @param wall 今見えている壁情報
	 */
	void addWall(int8_t x, int8_t y, MazeAngle angle, Walldata wall);

	/* /\** */
	/*  * @brief 壁を設定します */
	/*  * @param x 壁を追加する区画のx座標 */
	/*  * @param y 壁を追加する区画のy座標 */
	/*  * @param angle 今自分が向いている絶対方向 */
	/*  * @param wall 今見えている壁情報 */
	/*  *\/ */
	/* void setWall(int x, int y, MouseAngle angle, Walldata wall); */

	/**
	 * @brief 壁を追加します。絶対方向でのみ指定が可能です。
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が向いている絶対方向
	 */
	void addSingleWall(int8_t x, int8_t y, MazeAngle angle);

	/* /\** */
	/*  * @brief 壁を設定します。絶対方向でのみ指定が可能です。 */
	/*  * @param x 壁を追加する区画のx座標 */
	/*  * @param y 壁を追加する区画のy座標 */
	/*  * @param angle 今自分が向いている絶対方向 */
	/*  * @param wall 今見えている壁情報 */
	/*  * @bug まだaddSingleWallと同じ関数 */
	/*  *\/ */
	/* void setSingleWall(int x, int y, MouseAngle angle, Walldata wall); */


	/**
	 * @brief 絶対方向から見て壁があるか確認します
	 * @param x 壁を追加する区画のx座標
	 * @param y 壁を追加する区画のy座標
	 * @param angle 今自分が見ている絶対方向
	 * @return 壁が存在したらtrue
	 */
	bool isExistWall(int8_t x, int8_t y, MazeAngle angle);


	/**
	 * @brief 到達マップを設定します
	 * @param x 到達設定する区画のx座標
	 * @param y 到達設定する区画のy座標
	 */
	void setReached(int8_t, int8_t);

	/**
	 * @brief 到達したか確認します
	 * @param x 到達確認する区画のx座標
	 * @param y 到達確認する区画のy座標
	 * @return 到達していたら1，していなかったら0を返します
	 */
	bool hasReached(int8_t, int8_t);


	/**
	 * @brief =演算子のオーバーロード。Mapクラスを代入します
	 * @bug できない
	 */
	Map& operator= (Map tmp){
		/* column = tmp.column; */
		/* row = tmp.row; */
		/* reached = tmp.reached; */
		/* return *this; */
	}
};
