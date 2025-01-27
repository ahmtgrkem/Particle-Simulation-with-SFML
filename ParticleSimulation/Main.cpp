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

	const int maksimum_obje_sayisi = 500;
	const float obje_spawn_sıklıgı = 0.05f;
	const sf::Vector2f obje_spawn_konumu = { 420.0f, 100.0f };
	const sf::Vector2f obje_spawn_hizi = { 0.0f, 120.0f };

	sf::Clock saat;

	while (pencere.isOpen())
	{
		while (const std::optional olay = pencere.pollEvent())
		{
			if (olay->getIf<sf::Event::Closed>()) {
				pencere.close();
			}
		}

		if (solver.objeleriGetir().size() < maksimum_obje_sayisi && saat.getElapsedTime().asSeconds() >= obje_spawn_sıklıgı) {
			saat.restart();
			Particle& obje = solver.objeEkle(obje_spawn_konumu, 10);
			solver.objeHizAta(obje, obje_spawn_hizi);
		}


		solver.guncelle();
		pencere.clear(sf::Color::White);
		renderer.render(solver);
		pencere.display();
	}
}