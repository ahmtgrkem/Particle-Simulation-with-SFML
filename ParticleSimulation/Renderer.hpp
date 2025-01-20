#pragma once
#include "Solver.hpp"

class Renderer {
public:
	Renderer(sf::RenderTarget& target_) : target{target_} {}

	void render(Solver& solver) const {
		sf::CircleShape circle{ 1.0f };
		circle.setPointCount(32);
		const std::vector<Particle>& objeler = solver.objeleriGetir();
		for (const auto& obje : objeler) {
			circle.setPosition(obje.konum);
			circle.setScale(sf::Vector2f(obje.yaricap, obje.yaricap));
			circle.setFillColor(sf::Color::Blue);
			target.draw(circle);
		}
	}
private:
	sf::RenderTarget& target;
};