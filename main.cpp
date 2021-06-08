#include "CardCounting.h"
#include<iostream>
#include<vector>
using namespace std;

int main() {
	CardCounting card(0.5);

	int play_count;
	cout << "Play Game : ";
	cin >> play_count;

	vector<int> Deck6(312);
	vector<int> dealer_card(14);
	vector<int> player1_card(14);
	vector<int> player2_card(14);

	int play = 0;
	int d_hs, p1_hs, p2_hs;
	int player1_win = 0, player2_win = 0;
	int player1_draw_lose = 0, player2_draw_lose = 0;
	int player1_money = 100000, player2_money = 100000;

	card.Shuffle(Deck6);
	for(int i = 0; i < play_count; i++) {
		if (card.get_curr_used_card() >= card.get_shuffle_num()) {
			card.Shuffle(Deck6); // 모든 card를 shuffle함
		}
		
		p1_hs = p2_hs = 1;
		card.Caller(Deck6, dealer_card, player1_card, player2_card); // dealer로 부터 card를 2장씩 받는다.
		while ((p1_hs + p2_hs) != 0) { // player들이 모두 stand를 하지 않은 경우 
			p1_hs = card.HitStatus(1, player1_card); // player1의 hit, status를 결정
			if (p1_hs) card.AddCard(Deck6, player1_card); // hit이면 card추가 획득
			p2_hs = card.HitStatus(2, player2_card); // player2의 hit, status를 결정
			if (p2_hs) card.AddCard(Deck6, player2_card); // hit이면 card추가 획득
		}
		while(d_hs = card.HitStatus(0, dealer_card) == 1)
			if (d_hs) card.AddCard(Deck6, dealer_card); // hit이면 card 추가 획득
		card.dealer_hidden_card_counting();
			
		/* 승자 패자를 결정 */
		int d_p1 = card.WinLoseDraw(dealer_card, player1_card); // player1의 승패를 결정
		if (d_p1 == 0) player1_win++;
		else player1_draw_lose++;

		int Betting_money = card.BetMoney(player1_card, 100); // player1의 betting 금액을 확률에 따라 결정
		player1_money += card.Betting(player1_card, d_p1, Betting_money); // 승패에 따른 수익을 얻음

		int d_p2 = card.WinLoseDraw(dealer_card, player2_card); // player2의 승패를 결정
		if (d_p2 == 0) player2_win++;
		else player2_draw_lose++;

		player2_money += card.Betting(player2_card, d_p2, 100); // player2의 승패에 따른 수익을 얻음
	}
	cout << "<< Player1 >>" << endl;
	cout << "승리 : " << player1_win << "	" << "무승부 및 패배 : " << player1_draw_lose << endl;
	cout << "승률 : " << (double)(player1_win) * 100.0 / (double)(play_count) << "%" << endl;
	cout << "Money : " << player1_money << endl << endl;

	cout << "<< Player2 >>" << endl;
	cout << "승리 : " << player2_win << "	" << "무승부 및 패배 : " << player2_draw_lose << endl;
	cout << "승률 : " << (double)(player2_win) * 100.0 / (double)(play_count) << "%" << endl;
	cout << "Money : " << player2_money << endl << endl;

	return 0;
}