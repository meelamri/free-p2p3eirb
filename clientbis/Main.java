//import ModeClient;
import java.util.*;

public class Main
{


 	public static void main (String[] args)
 	{

 		// Configuration
 		Parser.config();


 		Bdd.init();

 		// Lancement du Client
 		SeederServ seederServ = new SeederServ();
		seederServ.start();
		ModeClient c = new ModeClient();   
		User u = new User();


	}
}


