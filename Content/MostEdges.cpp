/**
 * Forms structure with the most edges (N iterations)
 * This stems from an interesting observation I made.
 */

#include <iostream>
#include <vector>
#include <queue>

struct Point{
	int x, y;
	int phase;
};

const int dx[4] = {0, 0, 1, -1}, dy[4] = {1, -1, 0, 0};

void makeStructure (int N, std::vector<std::vector<bool>>& grid){
	if (N == 0) return;

	std::queue<Point> prt;
	Point start, on, next;
	int goodPlace;
	start.x = N+1, start.y = N+1, start.phase = 1;
	grid[N+1][N+1] = true;
	prt.push(start);

	while (!prt.empty()){
		on = prt.front();
		prt.pop();

		for (int i = 0; i < 4; i++){
			next.x = on.x + dx[i], next.y = on.y + dy[i], next.phase = on.phase + 1;
			goodPlace = 0;
			for (int d = 0; d < 4; d++){
				if (grid[next.y + dy[d]][next.x + dx[d]]){
					goodPlace++;
				}
			}
			if (!grid[next.y][next.x] && goodPlace == 1 &&  next.phase <= N){
				grid[next.y][next.x] = true;
				prt.push(next);
			}
		}
	}
}

signed main(){
	int N;
	std::cout << "Enter number of iterations:\n";
	std::cin >> N;
	std::vector<std::vector<bool>> square (1 + 2 * (N+1), std::vector<bool> (1 + 2 * (N+1)));
	makeStructure(N, square);

	for (std::vector<bool> i : square){
		for (bool d : i){
			std::cout << (d ? 'x' : ' ') << ' ';
		}
		std::cout << '\n';
	}
}
