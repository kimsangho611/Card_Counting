#include "CardCounting.h"

void CardCounting::Shuffle(vector<int>& Deck) {
	using_card = 0;
	Deck.assign(312, 0); // 6 deck card 초기화
	int index = 0;
	for (int i = 1; i <= 6; i++) { // 6 Deck
		for (int j = 1; j <= 4; j++) { // 4 shape 
			for (int k = 1; k <= 13; k++) { // card value (A, 2, 3, 4, ..., 10, j, q, k)
				Deck[index] = k;
				index++;
			}
		}
	}
	for (int i = 0; i < 50; i++) { // 총 50번 shuffle을 함
		random_device rd;
		default_random_engine rng(rd());
		shuffle(Deck.begin(), Deck.end(), rng);
	}
}

int CardCounting::HitStatus(int player, vector<int>&card) {
	int dealer_bust_threshold, player_bust_threshold;
	double dealer_bust_prob, player_bust_prob = 0;
	int card_sum = SumOfNum(card); // 현재 player가 가지고 있는 
	if (player == 0 || player == 2) { // dealer 이거나 player2인 경우 
		if (card_sum >= 17) return STAND;
		else return HIT;
	}
	else { // player 1인 경우
		if (card_sum >= 21) return STAND;
		if (card_sum <= 11) return HIT;

		int d_over_threshold_sum = 0, p_over_threshold_sum = 0;
		int assume_dealer_card_sum;
		int using_card_count = accumulate(card_counting.begin(), card_counting.end(), 0); // 전체 사용한 card 수를 구함

		if (dealer_open_card == 1) assume_dealer_card_sum = 10 + 11; // dealer의 open card가 A인 경우 
		else if (dealer_open_card >= 10) assume_dealer_card_sum = 10 + 10; // dealer의 open card가 10 이상인 경우 
		else assume_dealer_card_sum = 10 + dealer_open_card; // dealer의 open card가 9 이하인 경우 

		if (assume_dealer_card_sum >= 17) { // dealer는 stand를 할 것이라 가정
			int win_min_threshold = assume_dealer_card_sum + 1 - card_sum;
			int win_max_threshold = 21 - card_sum;

			if (assume_dealer_card_sum != 21) { // dealer card의 추정치가 17 ~ 20인 경우
				if (assume_dealer_card_sum >= card_sum) {
					for (int i = win_min_threshold; i <= win_max_threshold; i++) // player가 이길 수 있는 확률을 구함
						p_over_threshold_sum += card_counting[i];
					player_bust_prob = (double)((6 * (win_max_threshold - win_min_threshold + 1) * 4) - p_over_threshold_sum) / (312 - using_card_count); // player가 이길 수 있는 확률
					if (player_bust_prob > player_hit_status_prop_threshold) return HIT; // player가 이길 확률이 높은 경우 card를 더 뽑음
					else return STAND;
				}
				else return STAND;
			}
			else { // dealer card의 추정치가 21인 경우
				return STAND;
			}
		}
		else { // dealer는 HIT을 할 것이라 가정
			dealer_bust_threshold = 22 - assume_dealer_card_sum;
			for (int i = dealer_bust_threshold; i < 14; i++)
				d_over_threshold_sum += card_counting[i]; // threshold 이상의 사용한 card 수를 구함
			dealer_bust_prob = (double)((6 * (14 - dealer_bust_threshold) * 4) - d_over_threshold_sum) / (312 - using_card_count);
			if (dealer_bust_prob > player_hit_status_prop_threshold) return STAND; // dealer가 hit로 알아서 bust할 가능성이 높기 때문에 그냥 stand
			else { // dealer가 bust할 확률이 적은 경우
				if (assume_dealer_card_sum < card_sum) {
					return STAND;
				}
				else {
					int win_min_threshold = assume_dealer_card_sum + 1 - card_sum; // player가 이길 수 있는 최소 threshold를 구함
					int win_max_threshold = 21 - card_sum; // player가 이길 수 있는 최대 threshold를 구함
					if (win_max_threshold == 10) {
						for (int i = win_min_threshold; i < 14; i++) // player가 이길 수 있는 확률을 구함
							p_over_threshold_sum += card_counting[i];
					}
					else {
						for (int i = win_min_threshold; i <= win_max_threshold; i++) // player가 이길 수 있는 확률을 구함
							p_over_threshold_sum += card_counting[i];
					}
					double player_win_prob = (double)((6 * (win_max_threshold - win_min_threshold + 1) * 4) - p_over_threshold_sum) / (312 - using_card_count);

					if (player_win_prob > player_hit_status_prop_threshold) return HIT; // player가 이길 확률이 높은 경우
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
		if (i == 1) CardSum += (11 * card[i]); // card가 A인 경우 
		else if (i >= 10) CardSum += (10 * card[i]); // card가 10, J, Q, K인 경우 
		else CardSum += (i * card[i]); // card가 10 미만 인 경우
	}
	if (isAce) { // Ace가 player에게 존재하고 
		for (int i = 0; i < isAce; i++) { 
			if (CardSum > 21) CardSum -= 10; // card 합이 21을 초과하는 경우 
		}
	}
	return CardSum;
}

void CardCounting::Caller(vector<int>& Deck, vector<int>& d, vector<int>& p1, vector<int>& p2){
	d.assign(14, 0);
	p1.assign(14, 0);
	p2.assign(14, 0);
	for (int i = 0; i < 2; i++) {
		d[Deck[using_card]] += 1; // dealer에게 card 분배
		if (i == 1) { // dealer의 open card는 두번째로 받은 card로 설정
			card_counting[Deck[using_card]] += 1;
			dealer_open_card = Deck[using_card]; // 2번째로 받은 card를 open 함
		}
		else dealer_hidden_card = Deck[using_card];
		using_card++;

		p1[Deck[using_card]] += 1; // player1에게 card 분배
		card_counting[Deck[using_card]] += 1;
		using_card++;

		p2[Deck[using_card]] += 1; // player2에게 card 분배
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
	if (sum_of_dealer_card < sum_of_player_card) { // player의 card 합이 더 큰 경우 
		if (sum_of_player_card > 21) return 1; // player의 card 합이 21 초과인 경우
		else return 0;
	}
	else { // dealer의 card 합이 더 큰 경우
		if (sum_of_dealer_card == sum_of_player_card) { // dealer와 player의 card 합이 같은 경우 
			if (sum_of_player_card > 21) return 1; // player의 card 합이 21 초과인 경우 
			else return 2; // player의 card 합이 21 초과가 아닌 경우 
		}
		else { // dealer와 player의 card 합이 다른 경우 
			if (sum_of_dealer_card > 21) { // dealer의 card 합이 21 초과인 경우 
				if (sum_of_player_card > 21) return 1; // player의 card 합이 21 초과인 경우 
				else return 0; // player의 card 합이 21 이하인 경우 
			}
			else return 1; // dealer의 card 합이 21 이하인 경우 
		}
	}
}

int CardCounting::Betting(vector<int>& player, int winlosedraw, int money) {
	int card_num = accumulate(player.begin(), player.end(), 0); // player의 card 수를 count
	int sum_of_player_card = SumOfNum(player); // player의 card 합을 구함
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
	int card_sum = SumOfNum(player1); // 현재 player가 가지고 있는 

	int d_over_threshold_sum = 0, p_over_threshold_sum = 0;
	int assume_dealer_card_sum;
	int using_card_count = accumulate(card_counting.begin(), card_counting.end(), 0); // 전체 사용된 card 수를 구함

	if (dealer_open_card == 1) assume_dealer_card_sum = 10 + 11; // dealer의 open card가 A인 경우 
	else if (dealer_open_card >= 10) assume_dealer_card_sum = 10 + 10; // dealer의 open card가 10 이상인 경우
	else assume_dealer_card_sum = 10 + dealer_open_card; // dealer의 open card가 10 이하 인 경우

	if (card_sum == 21) return initial_balance * 2; // 초기 betting 금액의 2배를 betting함
	else if (card_sum > 21) return initial_balance; // betting을 하지 않음
	else {
		dealer_bust_threshold = 22 - assume_dealer_card_sum; // dealer의 bust되는 card의 threshold를 구함
		for (int i = dealer_bust_threshold; i < 14; i++)
			d_over_threshold_sum += card_counting[i]; // threshold 이상의 card가 사용된 수를 구함
		dealer_bust_prob = (double)((6 * (14 - dealer_bust_threshold) * 4) - d_over_threshold_sum) / (312 - using_card_count); // dealer가 bust할 확률을 구함
		return initial_balance * (1 + player_win_prob);
	}
}