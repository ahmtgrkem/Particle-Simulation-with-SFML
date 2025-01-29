#include "Renderer.hpp"
#include <cstring>

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
	solver.zamanAdimiAta(kare_hizi);

	const int maksimum_obje_sayisi = 1000;
	const float obje_spawn_sıklıgı = 0.05f;
	const sf::Vector2f obje_spawn_konumu = { 420.0f, 100.0f };
	const sf::Vector2f obje_spawn_hizi = { 0.0f, 120.0f };

	sf::Clock saat, fps_zamanlayici;
	float fps = 0.0f;
	bool spawn_durdu = false;

	while (pencere.isOpen())
	{
		while (const std::optional olay = pencere.pollEvent())
		{
			if (olay->getIf<sf::Event::Closed>()) {
				pencere.close();
			}
		}

		float elapsedTime = fps_zamanlayici.restart().asSeconds();
		float fps = 1.0f / elapsedTime;

		const bool fps_yeterli = (fps >= 40.0f);
		const bool maksimuma_ulasilmadi = (solver.objeleriGetir().size() < maksimum_obje_sayisi);

		std::string title = "FPS: " + std::to_string(fps) + " | Obje Sayisi: " + std::to_string(solver.objeleriGetir().size());
		if (!maksimuma_ulasilmadi) title += " | MAX";
		

		if (maksimuma_ulasilmadi && saat.getElapsedTime().asSeconds() >= obje_spawn_sıklıgı) {
			saat.restart();
			Particle& obje = solver.objeEkle(obje_spawn_konumu, 5);
			solver.objeHizAta(obje, obje_spawn_hizi);
		}

		solver.guncelle();
		pencere.clear(sf::Color::White);
		renderer.render(solver);
		pencere.setTitle(title.c_str());
		pencere.display();
	}
}