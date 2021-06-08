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
	vector<int> DeadCard; // �ش� ���ӿ��� ���� ī�带 �����ϴ� ����
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

	void Shuffle(vector<int>& card); // ī�带 ���� �Լ�
	int HitStatus(int player, vector<int>& card); // HIT, Status�� �����ϴ� �Լ�
	int SumOfNum(vector<int>& card); // player�� ���� card�� ���� ���ϴ� �Լ�

	void Caller(vector<int>& card, vector<int>& d, vector<int>& p1, vector<int>& p2); // ī�带 ����ϴ� �Լ�
	void AddCard(vector<int>& card, vector<int>& player); // �߰����� card�� �����ϴ� �Լ�

	int WinLoseDraw(vector<int>& dealer, vector<int>& player); // player�� ���и� �����ϴ� �Լ�

	int Betting(vector<int>& player, int winlosedraw, int money); // player�� ���п� card ���� ���� ���� ���� �����ϴ� �Լ�
	int BetMoney(vector<int>& player1, int initial_balance); // player1�� betting �ݾ��� �����ϴ� �Լ�
};


