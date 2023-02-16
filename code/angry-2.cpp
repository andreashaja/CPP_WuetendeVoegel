#include <iostream>
#include <string>
#include <vector>
#include <cmath>
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
    // Konstruktor inkl. Aufbau des Spielfelds
    GameArea(int num_rows, int num_cols)
    {
        // Spielfeld-Größe festlegen
        m_num_rows = num_rows;
        m_num_cols = num_cols;
        cout << "Spielfeld mit " << num_rows
             << " Zeilen und " << num_cols << " Spalten\n";

        // Bodenebene und Schleuder positionieren
        m_ground_level = m_num_rows - 1;
        m_slingshot_pos = 5;

        // Spielfeld initialisieren
        m_game_area.resize(m_num_rows,vector<char>(m_num_cols,' '));

        // Bodenebene einzeichnen
        for (int col = 0; col < m_num_cols; ++col)
        {
            m_game_area[m_ground_level][col] = '_';
        }

        // Schleuder einzeichnen
        m_game_area[m_ground_level][m_slingshot_pos] = '|';
        m_game_area[m_ground_level - 1][m_slingshot_pos] = '|';
        m_game_area[m_ground_level - 2][m_slingshot_pos - 1] = '\\';
        m_game_area[m_ground_level - 2][m_slingshot_pos + 1] = '/';
        m_game_area[m_ground_level - 3][m_slingshot_pos - 2] = '\\';
        m_game_area[m_ground_level - 3][m_slingshot_pos + 2] = '/';
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

        // y-Koordinate anpassen, da y=0 am unteren Rand sein soll
        object->m_position.y = m_ground_level-object->m_position.y;

        // Objekt zeichnen
        DrawObject(object, object->m_symbol);
    }

    // Objekt mit Symbol in Spielfeld einzeichnen
    void DrawObject(GameObject *object, char symbol)
    {
        // Nur im darstellbaren Bereich zeichnen
        int col = round(object->m_position.x);
        int row = round(object->m_position.y);
        if (row>=0 && row<m_num_rows && col>=0 && col<m_num_cols)
            m_game_area[row][col] = symbol;
    }

    // Den Inhalt des Spiefelds im Terminal ausgeben
    void PrintGameArea()
    {
        // Spielfeld in Kommandozeile ausgeben
        for (int y = 0; y < m_num_rows; y++)
        {
            for (int x = 0; x < m_num_cols; x++)
            {
                cout << m_game_area[y][x];
            }
            cout << endl;
        }
    }

    // Den nächsten Vogel in die Schleuder setzen
    Bird *GetNextBird()
    {
        // Nach noch "unbenutztem" Vogel suchen
        Bird *next_bird = nullptr;
        for (Bird *bird : m_game_birds)
        {
            if (bird->m_has_been_used == false)
            {
                next_bird = bird; // neuer Vogel gefunden
                next_bird->m_has_been_used = true;
                break; // Schleife verlassen
            }
        }

        // Vogel in Schleuder positionieren
        if (next_bird != nullptr)
        {
            DrawObject(next_bird, '_'); // Alte Position löschen
            next_bird->m_position.x = m_slingshot_pos;
            next_bird->m_position.y = m_ground_level - 2;
            DrawObject(next_bird, next_bird->m_symbol);
            cout << next_bird->m_attack_cry << endl;
        }
        return next_bird;
    }

    // Member-Variablen
    vector<Bird *> m_game_birds;      // Liste aller Vögel
    vector<Pig *> m_game_pigs;        // Liste aller Schweine
    int m_num_cols;                   // Feldgröße in x (Zelle = 1m)
    int m_num_rows;                   // Feldröße in y
    
    vector<vector<char>> m_game_area; // 2D-Spielfeld
    int m_slingshot_pos;              // x-Position der Schleuder
    int m_ground_level;               // Lage der Bodenebene
};

/*******************/
int main()
{
    // Spielobjekte erzeugen
    Bird b1(0.0, 0.0), b2(2.0, 0.0), b3(4.0, 0.0);
    Pig p1(30.0, 0.0), p2(50.0, 0.0, 1000);

    // Spielobjekte erzeugen
    int rows{15}, cols{100};
    GameArea area(rows, cols);

    // Spielwelt bevölkern
    area.AddGameObject(&p1);
    area.AddGameObject(&p2);
    area.AddGameObject(&b1);
    area.AddGameObject(&b2);
    area.AddGameObject(&b3);

    // Ersten Vogel in Schleuder laden
    Bird *next_bird = area.GetNextBird();

    // Spielfeld in Kommandozeile ausgeben
    area.PrintGameArea();

    return 0;
}