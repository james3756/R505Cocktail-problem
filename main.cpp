// Bar à cocktails : le problème des fumeurs (Little Book of Semaphores, 4.5) avec d'autres noms.
// Le serveur pose 2 ingrédients, le barman qui a le 3e doit faire le cocktail.
// Version sans correction : le programme finit par se bloquer (interblocage).
// C++20 standard : counting_semaphore (<semaphore>) et thread.

#include <chrono>
#include <iostream>
#include <semaphore>
#include <syncstream>
#include <thread>

using namespace std;

// <10> : valeur maximale que le sémaphore peut atteindre (ici il ne dépasse jamais 1)
counting_semaphore<10> serveurSem(1);
counting_semaphore<10> rhum(0);
counting_semaphore<10> menthe(0);
counting_semaphore<10> citron(0);

// osyncstream(cout) écrit la ligne entière d'un coup,
// pour que les messages de plusieurs threads ne se mélangent pas

// le serveur : chaque thread pose toujours la même paire
void serveurA()
{
    while (true) {
        serveurSem.acquire();
        osyncstream(cout) << "Serveur A pose : rhum + menthe" << endl;
        rhum.release();
        menthe.release();
    }
}

void serveurB()
{
    while (true) {
        serveurSem.acquire();
        osyncstream(cout) << "Serveur B pose : menthe + citron" << endl;
        menthe.release();
        citron.release();
    }
}

void serveurC()
{
    while (true) {
        serveurSem.acquire();
        osyncstream(cout) << "Serveur C pose : rhum + citron" << endl;
        rhum.release();
        citron.release();
    }
}

// les barmen
void barmanAvecRhum()
{
    int cocktails = 0;
    while (true) {
        menthe.acquire();
        osyncstream(cout) << "Barman avec rhum : a pris la menthe, attend le citron..." << endl;
        citron.acquire(); // peut rester bloqué ici
        osyncstream(cout) << "Barman avec rhum : fait son cocktail" << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        serveurSem.release();
        osyncstream(cout) << "Barman avec rhum : sert son cocktail, total : " << ++cocktails << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

void barmanAvecMenthe()
{
    int cocktails = 0;
    while (true) {
        rhum.acquire();
        osyncstream(cout) << "Barman avec menthe : a pris le rhum, attend le citron..." << endl;
        citron.acquire(); // peut rester bloqué ici
        osyncstream(cout) << "Barman avec menthe : fait son cocktail" << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        serveurSem.release();
        osyncstream(cout) << "Barman avec menthe : sert son cocktail, total : " << ++cocktails << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

void barmanAvecCitron()
{
    int cocktails = 0;
    while (true) {
        rhum.acquire();
        osyncstream(cout) << "Barman avec citron : a pris le rhum, attend la menthe..." << endl;
        menthe.acquire(); // peut rester bloqué ici
        osyncstream(cout) << "Barman avec citron : fait son cocktail" << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        serveurSem.release();
        osyncstream(cout) << "Barman avec citron : sert son cocktail, total : " << ++cocktails << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

int main()
{
    thread threads[] = {
        thread(serveurA), thread(serveurB), thread(serveurC),
        thread(barmanAvecRhum), thread(barmanAvecMenthe),
        thread(barmanAvecCitron)
    };
    // les threads ne s'arrêtent jamais : main attend ici pour toujours
    for (thread &t : threads)
        t.join();

    return 0;
}
