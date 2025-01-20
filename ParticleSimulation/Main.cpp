#include "Renderer.hpp"

int main() {
	constexpr int32_t pencere_genislik = 840;
	constexpr int32_t pencere_yukseklik = 840;

	sf::ContextSettings ayarlar;
	ayarlar.antiAliasingLevel = 1;

	sf::RenderWindow pencere(sf::VideoMode({pencere_genislik, pencere_yukseklik}),"SFML", sf::State::Windowed, ayarlar);
	
	const uint32_t kare_hizi = 60;
	pencere.setFramerateLimit(kare_hizi);

	Renderer renderer{ pencere };

	Solver solver;

	while (pencere.isOpen())
	{
		while (const std::optional olay = pencere.pollEvent())
		{
			if (olay->getIf<sf::Event::Closed>()) {
				pencere.close();
			}
		}

		solver.guncelle();
		pencere.clear(sf::Color::White);
		renderer.render(solver);
		pencere.display();
	}
}