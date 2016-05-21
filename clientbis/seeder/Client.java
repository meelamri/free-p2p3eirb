import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;

public abstract class Client 
{


	
	//ArrayList<String> looks;
 	//try 
 	//	{
 		static  InetAddress ipServer; 
 		public Socket socketServer;
		public PrintWriter outStream;// = new BufferedOutputStream(Files.newOutputStream(p, CREATE, APPEND))	
		public BufferedReader inStream;
		int portServer;
		//Treatments trs;
		//Parser parser;

 	//	}
 	
	// public Client (int port){
	// 	;
	// }		
	
	public void openConnection()
	{
		try {
		ipServer = InetAddress.getLocalHost();
		//ipServer = InetAddress.getByName("10.0.2.15");
		this.socketServer = new Socket(ipServer, portServer);
		this.outStream = new PrintWriter(new BufferedWriter(new OutputStreamWriter(this.socketServer.getOutputStream())), true);
		this.inStream = new BufferedReader(new InputStreamReader(this.socketServer.getInputStream()));
		this.sendToServer(Treatments.announceTreatment());
		if (! this.receiveFromServer().equals("ok")){
			 String string = this.receiveFromServer();
		// System.out.println(string + " blabla");
			 System.out.println("Tracker's response to announce gone wrongly \n");
			}	
		// trackerConnexion = true; 
		System.out.println("Connexion !!! established");
		// }
		}
		catch(IOException e){}
		
	}

	public void sendToServer (String s)
	{
		if(this.socketServer == null || !this.socketServer.isConnected())
		{
			System.out.println("Socket Error");
			return;
		}

		this.outStream.print(s);
		this.outStream.flush();
	}

	//public String receiveFromTracker() {}
		
	public String receiveFromServer()
	{
		if(this.socketServer == null || !this.socketServer.isConnected())
		{
			System.out.println("Socket not connected.");
			return null;
		}

		try{
			BufferedReader receive_s = this.inStream;
			String str = receive_s.readLine();
			//Tools.treatResponse(str);
			//Tools.traceFluxReceive(str);
			return str;
		}catch(IOException e){ return null; }
	}

    

}