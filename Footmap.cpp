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
bool Footmap::resetFootmap(){
	for(int i=0; i<32; ++i){
		for(int j=0; j<32; ++j) map[i][j] = 1024;
	}
}

/**
 * @brief 歩数マップを取得します
 * @param x 取得するx座標
 * @param y 取得するy座標
 * @param out 座標外の返り値
 * @return 設定した座標の歩数
 */
uint16_t Footmap::getFootmap(const int8_t x, const int8_t y, const uint16_t out){
	if(isOutside(x, y)) return out;
	else return map[x][y];
}

/**
 * @brief 歩数マップを設定します
 * @param x 設定するx座標
 * @param y 設定するy座標
 * @param data 設定する歩数
 */
bool Footmap::setFootmap(const int8_t x, const int8_t y, const uint16_t data){
	if(isOutside(x, y)) return false;
	else map[x][y] = data;
	return true;
}

bool Footmap::isOutside(const int8_t x, const int8_t y){
	if(x < 0 || x > 31 || y < 0 || y > 31) return true;
	else return false;
}

uint16_t Footmap::getMinNextTo(const int8_t x, const int8_t y, Walldata wall){
	uint16_t min = 1024;
	if(getFootmap(x-1, y, 1024) < min && (wall.isExistWall(MouseAngle::LEFT)==false))
		min = getFootmap(x-1, y, 1024);
	if(getFootmap(x+1, y, 1024) < min && (wall.isExistWall(MouseAngle::RIGHT)==false))
		min = getFootmap(x+1, y, 1024);
	if(getFootmap(x, y-1, 1024) < min && (wall.isExistWall(MouseAngle::BACK)==false))
		min = getFootmap(x, y-1, 1024);
	if(getFootmap(x, y+1, 1024) < min && (wall.isExistWall(MouseAngle::FRONT)==false))
		min = getFootmap(x, y+1, 1024);
	return min;
}

Footmap::~Footmap(){

}

