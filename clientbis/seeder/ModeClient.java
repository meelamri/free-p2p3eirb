import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;

public class ModeClient extends Client implements Runnable
{


	// final int portTracker = 7777;
	// //ArrayList<String> looks;
 // 	//try 
 // 	//	{
 // 		static  InetAddress ipTracker; 
 // 		private Socket socketTracker;
	// 	private PrintWriter outStream;// = new BufferedOutputStream(Files.newOutputStream(p, CREATE, APPEND))	
	// 	private BufferedReader inStream;
	// 	boolean trackerConnexion;
	// 	Thread thread;
	// 	//Treatments trs;
	// 	//Parser parser;

 // 	//	}
 	Thread thread;
					
	public ModeClient()
	{
		//ipTracker = InetAddress.getByName("10.1.6.218");
		portServer = 8080;
		thread = new Thread (this);	
		thread.start();

	}

	public void run(){
		startTreatment();
	}

	// public void openConnection()
	// {
	// 	try {
	// 	ipTracker = InetAddress.getLocalHost();
	// 	//ipTracker = InetAddress.getByName("10.1.6.218");
	// 	this.socketTracker = new Socket(ipTracker, portTracker);
	// 	this.outStream = new PrintWriter(new BufferedWriter(new OutputStreamWriter(this.socketTracker.getOutputStream())), true);
	// 	this.inStream = new BufferedReader(new InputStreamReader(this.socketTracker.getInputStream()));
	// 	}
	// 	catch(IOException e){}
		
	// }

	// public void sendToServer (String s)
	// {
	// 	if(this.socketTracker == null || !this.socketTracker.isConnected())
	// 	{
	// 		System.out.println("Socket Error");
	// 		return;
	// 	}

	// 	this.outStream.print(s);
	// 	this.outStream.flush();
	// }

	// //public String receiveFromTracker() {}
		
	// public String receiveFromServer()
	// {
	// 	if(this.socketTracker == null || !this.socketTracker.isConnected())
	// 	{
	// 		System.out.println("Socket not connected.");
	// 		return null;
	// 	}

	// 	try{
	// 		BufferedReader receive_s = this.inStream;
	// 		String str = receive_s.readLine();
	// 		//Tools.treatResponse(str);
	// 		//Tools.traceFluxReceive(str);
	// 		return str;
	// 	}catch(IOException e){ return null; }
	// }

    
	public void startTreatment ()
	{
		// Opening the connexion
		System.out.print("Launching the program !!!\n");
		this.openConnection();

		// Sending the annouce
		this.sendToServer(Treatments.announceTreatment());
		System.out.println(this.receiveFromServer());
		// if (! this.receiveFromServer().equals("ok")){
		// 		System.out.println("Tracker's response to announce gone wrongly \n");
		// 		System.exit(0);
		// 	}

		//String string = this.receiveFromTracker();
		//System.out.println("Connexion established");
		
		Enumeration<String> strEnum;
		String keyTable;
		int x =1;
		String str;	
		// waiting for the user to ask for the files
		synchronized (Bdd.class){
			while (Bdd.isEmptyFilesUser()){
			    UserClientLock.staticWait();
	    	    }
				 
	    	    //  filling the (filename, filepeer) table
				
				System.out.println("OVER HERE before wait\n");
				Bdd.bddUser();
				strEnum = Collections.enumeration(Bdd.keySetFilesUser());
		 		while(strEnum.hasMoreElements()) 
		 			{
		 				keyTable = strEnum.nextElement();
		 				//System.out.println("OVER HERE \n");
		 				System.out.println(Treatments.lookTreatment(keyTable));

		 				sendToServer(Treatments.lookTreatment(keyTable));
		 				//System.out.println("Key :" + keyTable + "FileName : " + Bdd.FILES_USER.get(keyTable).filename + " \n" );
						str = receiveFromServer();
						System.out.println(str);
						
						Parser.treatResp(str);
		 				//Parser.treatResp("list [a.dat 8787 1024 12121212121212121212121212121212]");
		 				//Parser.treatResp("list [b.dat 543555 1024 21212121212121212121212121212121]");

		 			}
		 		}
		 	

		 // filling the (key, peers) table
		strEnum = Collections.enumeration(Bdd.FILES_TRACKER.keySet());
 		while(strEnum.hasMoreElements()) 
 			{
 				keyTable = strEnum.nextElement();
 				System.out.println(Treatments.getFileTreatment(keyTable));
 				sendToServer(Treatments.getFileTreatment(keyTable));
 				//System.out.println("Key :" + keyTable + "FileName : " + Bdd.FILES_TRACKER.get(keyTable).filename + "\n" );
 				Parser.treatResp(receiveFromServer());
				//System.out.println("Tessst\n");

	       	}

	    // starting the download of the each key
	    strEnum = Collections.enumeration(Bdd.KEY_TO_PEER.keySet());
 		while(strEnum.hasMoreElements()) 
 			{
 				keyTable = strEnum.nextElement();
 				new Leech(keyTable);

	       	}
	    
	   //Bdd.bddPeer();
		//Bdd.bddUser();
		//Bdd.bddSeed();
		
	}
}
