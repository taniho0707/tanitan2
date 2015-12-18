/**
 * 歩数マップを管理するクラス
 */
#include "Footmap.h"

Footmap::Footmap(){
	resetFootmap();
}

/**
 * @brief 歩数マップをクリアします
 */
void Footmap::resetFootmap(){
	for(int i=0; i<32; ++i){
		for(int j=0; j<32; ++j) map[i][j] = 255;
	}
}

/**
 * @brief 歩数マップを取得します
 * @param x 取得するx座標
 * @param y 取得するy座標
 * @param out 座標外の返り値
 * @return 設定した座標の歩数
 */
int Footmap::getFootmap(int x, int y, int out){
	if(isOutside(x, y)) return out;
	else return map[x][y];
}

/**
 * @brief 歩数マップを設定します
 * @param x 設定するx座標
 * @param y 設定するy座標
 * @param data 設定する歩数
 */
void Footmap::setFootmap(int x, int y, int data){
	if(isOutside(x, y)) return;
	if(data > 255) map[x][y] = 255;
	else map[x][y] = data;
	return;
}

bool Footmap::isOutside(int x, int  y){
	if(x < 0 || x > 31 || y < 0 || y > 31) return true;
	else return false;
}

Footmap::~Footmap(){

}

