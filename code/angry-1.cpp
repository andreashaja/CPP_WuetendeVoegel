#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Datenstruktur für Koordinaten
struct Tuple2D
{
    double x, y;
};

// Liste relevanter Objektarten
enum ObjType
{
    BIRD,
    PIG,
    GROUND,
    AIR
};

// Basisklasse für Spielobjekte
class GameObject
{
public:
    // Konstruktor mit Pflicht-Parametern
    GameObject(double pos_x, double pos_y, ObjType type, char sym)
    {
        m_position.x = pos_x;
        m_position.y = pos_y;
        m_type = type;
        m_symbol = sym;
    }

    // Member-Variablen
    ObjType m_type;
    char m_symbol{' '};
    Tuple2D m_position{0.0, 0.0}; // x, y
    Tuple2D m_velocity{0.0, 0.0}; // v_x, v_y
    bool m_has_been_used{false};
};

// Kindklasse für Vogel-Objekte
class Bird : public GameObject
{
public:
    // Konstruktor mit Aufruf des Basis-Konstruktors
    Bird(double pos_x, double pos_y)
        : GameObject(pos_x, pos_y, BIRD, 'o')
    {
        cout << "Vogel an x=" << m_position.x
             << ", y=" << m_position.y << endl;
    }

    // Member-Variablen
    string m_attack_cry{"Angriff!"};
};

// Kindklasse für Schweine-Objekte
class Pig : public GameObject
{
public:
    // Konstruktor mit Aufruf des Basis-Konstruktors
    Pig(double pos_x, double pos_y, int score = 500)
        : GameObject(pos_x, pos_y, PIG, '@')
    {
        m_score = score;
        cout << "Schwein an x=" << m_position.x
             << ", y=" << m_position.y << " ("
             << m_score << " Punkte)\n";
    }

    // Member-Variablen
    int m_score{500};
    string m_hit_cry{"Oh nein!"};
};

// Klasse für das Spielfeld-Objekt
class GameArea
{
public:
    // Konstruktor (noch ohne) Aufbau des Spielfelds
    GameArea(int num_rows, int num_cols)
    {
        // Spielfeld-Größe festlegen
        m_num_rows = num_rows;
        m_num_cols = num_cols;
        cout << "Spielfeld mit " << num_rows
             << " Zeilen und " << num_cols << " Spalten\n";
    }

    // Hinzufügen von Spielobjekten in die jeweiligen Listen
    void AddGameObject(GameObject *object)
    {
        // Objekt in passende Liste hinzufügen
        if (object->m_type == BIRD)
            m_game_birds.push_back((Bird *)object);
        else if (object->m_type == PIG)
            m_game_pigs.push_back((Pig *)object);

        cout << "Objekt-Typ " << object->m_type << " hinzugefügt ("
             << m_game_birds.size() << " Vögel, "
             << m_game_pigs.size() << " Schweine)\n";
    }

    // Member-Variablen
    vector<Bird *> m_game_birds; // Liste aller Vögel im Spiel
    vector<Pig *> m_game_pigs;   // Liste aller Schweine im Spiel
    int m_num_cols;              // Feldgröße in x (Zelle = 1m)
    int m_num_rows;              // Feldröße in y
};

/*******************/
int main()
{
    // Spielobjekte erzeugen
    Bird b1(0.0, 0.0), b2(2.0, 0.0), b3(4.0, 0.0);
    Pig p1(30.0, 0.0), p2(50.0, 0.0, 1000);

    // Spielwelt erzeugen
    int rows{15}, cols{100};
    GameArea area(rows, cols);

    // Spielwelt bevölkern
    area.AddGameObject(&p1);
    area.AddGameObject(&p2);
    area.AddGameObject(&b1);
    area.AddGameObject(&b2);
    area.AddGameObject(&b3);

    return 0;
}