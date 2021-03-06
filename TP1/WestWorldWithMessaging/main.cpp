#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Swain.h"
#include "BarFly.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#include "GUI.h"

std::ofstream os;

int main()
{

//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  //create a swain
  Swain* JeanErnestain = new Swain(ent_Swain);

  //create a BarFly
  BarFly* Bernard = new BarFly(ent_BarFly);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(JeanErnestain);
  EntityMgr->RegisterEntity(Bernard);

  //Create a mutex shared between the threads
  sf::Mutex protector;
  
  //Create one thread for each entity
  sf::Thread BobThread([&] {
	  for (int i = 0; i < 30; ++i)
	  {
			protector.lock();
			Bob->Update();
			Dispatch->DispatchDelayedMessages();
			protector.unlock();
			Sleep(800);
	  }
  });
  sf::Thread ElsaThread([&] {
	  for (int i = 0; i < 30; ++i)
	  {
		  protector.lock();
		  Elsa->Update();
		  protector.unlock();
		  Sleep(800);
	  }
  });
  sf::Thread JeanThread([&] {
	  for (int i = 0; i < 30; ++i)
	  {
		  protector.lock();
		  JeanErnestain->Update();
		  protector.unlock();
		  Sleep(800);
	  }
  });
  sf::Thread BernardThread([&]{ 
	  for (int i = 0; i < 30; ++i)
	  {
		  protector.lock();
		  Bernard->Update();
		  protector.unlock();
		  Sleep(800);
	  }
  });
  

  //Launch the threads
  BobThread.launch();
  ElsaThread.launch();
  JeanThread.launch();
  BernardThread.launch();

  //Wait for all of the threads to finish
  BobThread.wait();
  ElsaThread.wait();
  JeanThread.wait();
  BernardThread.wait();

  //tidy up
  delete Bob;
  delete Elsa;
  delete JeanErnestain;
  delete Bernard;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();
 
  return 0;
}






