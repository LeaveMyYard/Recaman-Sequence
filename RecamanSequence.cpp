#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>

void drawLine(sf::Vector2f from, sf::Vector2f to, sf::RenderWindow* window){
	sf::Vertex line[2] =
	{
		sf::Vertex(from),
		sf::Vertex(to)
	};

	window->draw(line, 2, sf::Lines);
}

void drawArc(float from, float to, bool down, sf::RenderWindow* window) {
	float center = (from + to) / 2;
	float radius = std::abs((to - from) / 2);
	float pi = 3.14159265359;
	int maxiter = 30;
	float lastx = to;
	float lasty = 100;
	for (int i = 0; i < maxiter; ++i) {
		float angle = pi / float(maxiter) * i;
		float y = 100 + radius * std::sin(angle) * (down ? 1 : -1);
		float x = center + radius * std::cos(angle);
		if (from > to && i == 0) {
			lastx = x;
			lasty = y;
			continue;
		}
		drawLine(sf::Vector2f(lastx, lasty), sf::Vector2f(x, y), window);
		lastx = x;
		lasty = y;
	}
	drawLine(sf::Vector2f(lastx, lasty), sf::Vector2f(from < to ? from : to, 100), window);
}

int main()
{
	int delay = 250, iterperframe = 1; // delay stands for amount of ms between frames, iterperframe for amount of points calculated per frame

	sf::RenderWindow window(sf::VideoMode(1200, 900), "Recaman Sequence!");
	std::vector<int> sequence;
	int maxnum = 3;
	int step = 1;
	int current = 0;
	sequence.push_back(current);
	bool down = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (int i = 0; i < iterperframe; ++i) {
			int next = 0;
			if (current - step > 1) {
				bool contains = std::find(sequence.begin(), sequence.end(), current - step) != sequence.end();
				next = contains ? current + step : current - step;
			}
			else {
				next = current + step;
			}

			std::cout << current << std::endl;

			down = false;
			current = next;
			sequence.push_back(current);
			step++;
			if (current > maxnum) maxnum = current;
		}

		sf::View view(sf::Vector2f(float(maxnum) / 2, 100.f), sf::Vector2f(float(maxnum), float(maxnum)));
		window.setView(view);
		window.clear();

		auto last = sequence.begin();
		for (auto it = sequence.begin(); it != sequence.end(); ++it) {
			down = !down;
			drawArc(*it, *last, down, &window);
			last = it;
		}

		window.display();
		sf::sleep(sf::milliseconds(delay));
	}

	return 0;
}