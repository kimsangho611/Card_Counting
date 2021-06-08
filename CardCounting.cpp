#include "CardCounting.h"

void CardCounting::Shuffle(vector<int>& Deck) {
	using_card = 0;
	Deck.assign(312, 0); // 6 deck card �ʱ�ȭ
	int index = 0;
	for (int i = 1; i <= 6; i++) { // 6 Deck
		for (int j = 1; j <= 4; j++) { // 4 shape 
			for (int k = 1; k <= 13; k++) { // card value (A, 2, 3, 4, ..., 10, j, q, k)
				Deck[index] = k;
				index++;
			}
		}
	}
	for (int i = 0; i < 50; i++) { // �� 50�� shuffle�� ��
		random_device rd;
		default_random_engine rng(rd());
		shuffle(Deck.begin(), Deck.end(), rng);
	}
}

int CardCounting::HitStatus(int player, vector<int>&card) {
	int dealer_bust_threshold, player_bust_threshold;
	double dealer_bust_prob, player_bust_prob = 0;
	int card_sum = SumOfNum(card); // ���� player�� ������ �ִ� 
	if (player == 0 || player == 2) { // dealer �̰ų� player2�� ��� 
		if (card_sum >= 17) return STAND;
		else return HIT;
	}
	else { // player 1�� ���
		if (card_sum >= 21) return STAND;
		if (card_sum <= 11) return HIT;

		int d_over_threshold_sum = 0, p_over_threshold_sum = 0;
		int assume_dealer_card_sum;
		int using_card_count = accumulate(card_counting.begin(), card_counting.end(), 0); // ��ü ����� card ���� ����

		if (dealer_open_card == 1) assume_dealer_card_sum = 10 + 11; // dealer�� open card�� A�� ��� 
		else if (dealer_open_card >= 10) assume_dealer_card_sum = 10 + 10; // dealer�� open card�� 10 �̻��� ��� 
		else assume_dealer_card_sum = 10 + dealer_open_card; // dealer�� open card�� 9 ������ ��� 

		if (assume_dealer_card_sum >= 17) { // dealer�� stand�� �� ���̶� ����
			int win_min_threshold = assume_dealer_card_sum + 1 - card_sum;
			int win_max_threshold = 21 - card_sum;

			if (assume_dealer_card_sum != 21) { // dealer card�� ����ġ�� 17 ~ 20�� ���
				if (assume_dealer_card_sum >= card_sum) {
					for (int i = win_min_threshold; i <= win_max_threshold; i++) // player�� �̱� �� �ִ� Ȯ���� ����
						p_over_threshold_sum += card_counting[i];
					player_bust_prob = (double)((6 * (win_max_threshold - win_min_threshold + 1) * 4) - p_over_threshold_sum) / (312 - using_card_count); // player�� �̱� �� �ִ� Ȯ��
					if (player_bust_prob > player_hit_status_prop_threshold) return HIT; // player�� �̱� Ȯ���� ���� ��� card�� �� ����
					else return STAND;
				}
				else return STAND;
			}
			else { // dealer card�� ����ġ�� 21�� ���
				return STAND;
			}
		}
		else { // dealer�� HIT�� �� ���̶� ����
			dealer_bust_threshold = 22 - assume_dealer_card_sum;
			for (int i = dealer_bust_threshold; i < 14; i++)
				d_over_threshold_sum += card_counting[i]; // threshold �̻��� ����� card ���� ����
			dealer_bust_prob = (double)((6 * (14 - dealer_bust_threshold) * 4) - d_over_threshold_sum) / (312 - using_card_count);
			if (dealer_bust_prob > player_hit_status_prop_threshold) return STAND; // dealer�� hit�� �˾Ƽ� bust�� ���ɼ��� ���� ������ �׳� stand
			else { // dealer�� bust�� Ȯ���� ���� ���
				if (assume_dealer_card_sum < card_sum) {
					return STAND;
				}
				else {
					int win_min_threshold = assume_dealer_card_sum + 1 - card_sum; // player�� �̱� �� �ִ� �ּ� threshold�� ����
					int win_max_threshold = 21 - card_sum; // player�� �̱� �� �ִ� �ִ� threshold�� ����
					if (win_max_threshold == 10) {
						for (int i = win_min_threshold; i < 14; i++) // player�� �̱� �� �ִ� Ȯ���� ����
							p_over_threshold_sum += card_counting[i];
					}
					else {
						for (int i = win_min_threshold; i <= win_max_threshold; i++) // player�� �̱� �� �ִ� Ȯ���� ����
							p_over_threshold_sum += card_counting[i];
					}
					double player_win_prob = (double)((6 * (win_max_threshold - win_min_threshold + 1) * 4) - p_over_threshold_sum) / (312 - using_card_count);

					if (player_win_prob > player_hit_status_prop_threshold) return HIT; // player�� �̱� Ȯ���� ���� ���
					else return STAND;
				}
			}
		}
		
	}
}

int CardCounting::SumOfNum(vector<int>& card) {
	int isAce = card[1];
	int CardSum = 0;
	for (int i = 1; i < 14; i++) {
		if (i == 1) CardSum += (11 * card[i]); // card�� A�� ��� 
		else if (i >= 10) CardSum += (10 * card[i]); // card�� 10, J, Q, K�� ��� 
		else CardSum += (i * card[i]); // card�� 10 �̸� �� ���
	}
	if (isAce) { // Ace�� player���� �����ϰ� 
		for (int i = 0; i < isAce; i++) { 
			if (CardSum > 21) CardSum -= 10; // card ���� 21�� �ʰ��ϴ� ��� 
		}
	}
	return CardSum;
}

void CardCounting::Caller(vector<int>& Deck, vector<int>& d, vector<int>& p1, vector<int>& p2){
	d.assign(14, 0);
	p1.assign(14, 0);
	p2.assign(14, 0);
	for (int i = 0; i < 2; i++) {
		d[Deck[using_card]] += 1; // dealer���� card �й�
		if (i == 1) { // dealer�� open card�� �ι�°�� ���� card�� ����
			card_counting[Deck[using_card]] += 1;
			dealer_open_card = Deck[using_card]; // 2��°�� ���� card�� open ��
		}
		else dealer_hidden_card = Deck[using_card];
		using_card++;

		p1[Deck[using_card]] += 1; // player1���� card �й�
		card_counting[Deck[using_card]] += 1;
		using_card++;

		p2[Deck[using_card]] += 1; // player2���� card �й�
		card_counting[Deck[using_card]] += 1;
		using_card++;
	}
}

void CardCounting::AddCard(vector<int>& Deck, vector<int>& player) {
	player[Deck[using_card]] += 1;
	card_counting[Deck[using_card]] += 1;
	using_card++;
}

int CardCounting::WinLoseDraw(vector<int>& dealer, vector<int>& player) {
	int sum_of_dealer_card = SumOfNum(dealer);
	int sum_of_player_card = SumOfNum(player);

	// 0 : win		1 : lose		2 : draw
	if (sum_of_dealer_card < sum_of_player_card) { // player�� card ���� �� ū ��� 
		if (sum_of_player_card > 21) return 1; // player�� card ���� 21 �ʰ��� ���
		else return 0;
	}
	else { // dealer�� card ���� �� ū ���
		if (sum_of_dealer_card == sum_of_player_card) { // dealer�� player�� card ���� ���� ��� 
			if (sum_of_player_card > 21) return 1; // player�� card ���� 21 �ʰ��� ��� 
			else return 2; // player�� card ���� 21 �ʰ��� �ƴ� ��� 
		}
		else { // dealer�� player�� card ���� �ٸ� ��� 
			if (sum_of_dealer_card > 21) { // dealer�� card ���� 21 �ʰ��� ��� 
				if (sum_of_player_card > 21) return 1; // player�� card ���� 21 �ʰ��� ��� 
				else return 0; // player�� card ���� 21 ������ ��� 
			}
			else return 1; // dealer�� card ���� 21 ������ ��� 
		}
	}
}

int CardCounting::Betting(vector<int>& player, int winlosedraw, int money) {
	int card_num = accumulate(player.begin(), player.end(), 0); // player�� card ���� count
	int sum_of_player_card = SumOfNum(player); // player�� card ���� ����
	if (winlosedraw == 0) {
		if (sum_of_player_card == 21) {
			if (card_num == 2) return ((double)money * 2.5) - money; // black jack
			else return ((double)money * 2.0) - money; // win
		}
		else {
			return ((double)money * 2.0) - money; // win
		}
	}
	else if (winlosedraw == 1) { // lose
		return -money;
	}
	else { // draw
		return 0;
	}
}

int CardCounting::BetMoney(vector<int>& player1, int initial_balance) {
	int dealer_bust_threshold, player_bust_threshold;
	double dealer_bust_prob, player_win_prob = 0;
	int card_sum = SumOfNum(player1); // ���� player�� ������ �ִ� 

	int d_over_threshold_sum = 0, p_over_threshold_sum = 0;
	int assume_dealer_card_sum;
	int using_card_count = accumulate(card_counting.begin(), card_counting.end(), 0); // ��ü ���� card ���� ����

	if (dealer_open_card == 1) assume_dealer_card_sum = 10 + 11; // dealer�� open card�� A�� ��� 
	else if (dealer_open_card >= 10) assume_dealer_card_sum = 10 + 10; // dealer�� open card�� 10 �̻��� ���
	else assume_dealer_card_sum = 10 + dealer_open_card; // dealer�� open card�� 10 ���� �� ���

	if (card_sum == 21) return initial_balance * 2; // �ʱ� betting �ݾ��� 2�踦 betting��
	else if (card_sum > 21) return initial_balance; // betting�� ���� ����
	else {
		dealer_bust_threshold = 22 - assume_dealer_card_sum; // dealer�� bust�Ǵ� card�� threshold�� ����
		for (int i = dealer_bust_threshold; i < 14; i++)
			d_over_threshold_sum += card_counting[i]; // threshold �̻��� card�� ���� ���� ����
		dealer_bust_prob = (double)((6 * (14 - dealer_bust_threshold) * 4) - d_over_threshold_sum) / (312 - using_card_count); // dealer�� bust�� Ȯ���� ����
		return initial_balance * (1 + player_win_prob);
	}
}