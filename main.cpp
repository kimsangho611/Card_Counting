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
			card.Shuffle(Deck6); // ��� card�� shuffle��
		}
		
		p1_hs = p2_hs = 1;
		card.Caller(Deck6, dealer_card, player1_card, player2_card); // dealer�� ���� card�� 2�徿 �޴´�.
		while ((p1_hs + p2_hs) != 0) { // player���� ��� stand�� ���� ���� ��� 
			p1_hs = card.HitStatus(1, player1_card); // player1�� hit, status�� ����
			if (p1_hs) card.AddCard(Deck6, player1_card); // hit�̸� card�߰� ȹ��
			p2_hs = card.HitStatus(2, player2_card); // player2�� hit, status�� ����
			if (p2_hs) card.AddCard(Deck6, player2_card); // hit�̸� card�߰� ȹ��
		}
		while(d_hs = card.HitStatus(0, dealer_card) == 1)
			if (d_hs) card.AddCard(Deck6, dealer_card); // hit�̸� card �߰� ȹ��
		card.dealer_hidden_card_counting();
			
		/* ���� ���ڸ� ���� */
		int d_p1 = card.WinLoseDraw(dealer_card, player1_card); // player1�� ���и� ����
		if (d_p1 == 0) player1_win++;
		else player1_draw_lose++;

		int Betting_money = card.BetMoney(player1_card, 100); // player1�� betting �ݾ��� Ȯ���� ���� ����
		player1_money += card.Betting(player1_card, d_p1, Betting_money); // ���п� ���� ������ ����

		int d_p2 = card.WinLoseDraw(dealer_card, player2_card); // player2�� ���и� ����
		if (d_p2 == 0) player2_win++;
		else player2_draw_lose++;

		player2_money += card.Betting(player2_card, d_p2, 100); // player2�� ���п� ���� ������ ����
	}
	cout << "<< Player1 >>" << endl;
	cout << "�¸� : " << player1_win << "	" << "���º� �� �й� : " << player1_draw_lose << endl;
	cout << "�·� : " << (double)(player1_win) * 100.0 / (double)(play_count) << "%" << endl;
	cout << "Money : " << player1_money << endl << endl;

	cout << "<< Player2 >>" << endl;
	cout << "�¸� : " << player2_win << "	" << "���º� �� �й� : " << player2_draw_lose << endl;
	cout << "�·� : " << (double)(player2_win) * 100.0 / (double)(play_count) << "%" << endl;
	cout << "Money : " << player2_money << endl << endl;

	return 0;
}