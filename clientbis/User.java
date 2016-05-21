import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;



public class User implements Runnable{
	BufferedReader in;
	Thread thread;
	

	public User(){
		thread = new Thread(this);
		in = new BufferedReader(new InputStreamReader(System.in));
		thread.start();
	}

	public void run (){
		synchronized (Bdd.class){
			FilePeer file;
			boolean controlVar = true;
			//this.openConnection();
			Scanner sc = new Scanner(System.in);
			String trackerResponse;
			int userResponse;
			String fileName;
			String keyTable;
			while (controlVar){
				System.out.println("Choose your next move: \n");
				System.out.println("Press 0 to start downloading");
				System.out.println("Press 1 for another file seeking");
				//System.out.println("Press 2 for a file size criteria");
				userResponse = sc.nextInt(); 	
				switch (userResponse){
					case 0:
						controlVar = false;
						System.out.println("OVER HERE before notify\n");
						UserClientLock.staticNotify();

						break;
					case 1: 
						System.out.println("File name?");
						sc.nextLine();
						fileName = sc.nextLine();
						file = new FilePeer(fileName);
						//Bdd.FILES_USER.put(fileName,file);
						Bdd.putFilesUser(fileName, file);
						// System.out.println("OVER HERE before notify\n");
						// UserClientLock.staticNotify();
						break;
					default:
						//System.exit(0);
						break;
				}		 
			
			}
			//System.out.println("OVER HERE before notify\n");
			//notify();
		
		}
	}
}