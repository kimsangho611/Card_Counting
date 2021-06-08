#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<numeric>
#define HIT		1
#define STAND	0

using namespace std;

class CardCounting {
private:
	vector<int> DeadCard; // 해당 게임에서 사용된 카드를 저장하는 변수
	vector<int> card_counting;

	double card_shuffle_num = 312 * 0.8;
	int using_card = 0;
	double player_hit_status_prop_threshold;
	int dealer_open_card, dealer_hidden_card;

public:
	CardCounting(double probability) {
		player_hit_status_prop_threshold = probability;
		card_counting.assign(14, 0);
		dealer_open_card = 0;
	}
	~CardCounting() {};

	double get_shuffle_num() {
		return this->card_shuffle_num;
	}
	double get_curr_used_card() {
		return this->using_card;
	}

	void dealer_hidden_card_counting() {
		card_counting[dealer_hidden_card]++;
	}

	void Shuffle(vector<int>& card); // 카드를 섞는 함수
	int HitStatus(int player, vector<int>& card); // HIT, Status를 결정하는 함수
	int SumOfNum(vector<int>& card); // player가 가진 card의 합을 구하는 함수

	void Caller(vector<int>& card, vector<int>& d, vector<int>& p1, vector<int>& p2); // 카드를 배분하는 함수
	void AddCard(vector<int>& card, vector<int>& player); // 추가적인 card를 결정하는 함수

	int WinLoseDraw(vector<int>& dealer, vector<int>& player); // player의 승패를 결정하는 함수

	int Betting(vector<int>& player, int winlosedraw, int money); // player의 승패와 card 값에 따라 수익 돈을 결정하는 함수
	int BetMoney(vector<int>& player1, int initial_balance); // player1의 betting 금액을 결정하는 함수
};


