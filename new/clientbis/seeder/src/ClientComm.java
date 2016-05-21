import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;

public class ClientComm extends Client {

	ArrayList<Integer> offsets;
	ArrayList<Integer> currentOffsets;
	BufferMapBis buff;
	Peer peer;
	String key;
	//byte [] currentBuff;
	public ClientComm(String key, Peer p, ArrayList<Integer> currentOffsets) {
		this.currentOffsets = currentOffsets;
		offsets = new ArrayList<Integer>();
		this.key = key;
		//portServer = p.numPort;
		portServer = 60001;
		//this.currentBuff = currentBuff;
		peer = p;
		System.out.println("numport : " + peer.numPort+ "ip : " + peer.ipAddr);
			}	
	// public ClientComm(ArrayList<Integer> offsets){
	// 	this.offsets = offsets;
	// 	this.currentOffsets = offsets;
	// }
	public void run() {
		startTreatment();
	}
	public void openConnection()
	{
		try {
		System.out.print("Opening a connexion for leeching \n");
		//ipServer = InetAddress.getLocalHost();
		ipServer = InetAddress.getByName(peer.ipAddr);
		//ipServer = InetAddress.getByName("10.1.6.218");
		this.socketServer = new Socket(ipServer, portServer);
		this.outStream = new PrintWriter(new BufferedWriter(new OutputStreamWriter(this.socketServer.getOutputStream())), true);
		this.inStream = new BufferedReader(new InputStreamReader(this.socketServer.getInputStream()));
		// this.sendToTracker(Treatments.announceTreatment());
		// //if (! this.receiveFromTracker().equals("ok"))
		// String string = this.receiveFromTracker();
		// System.out.println(string + " blabla");
		// 		System.out.println("Tracker's response to announce gone wrongly \n");
		// trackerConnexion = true; 
		// System.out.println("Connexion established");
		// }
		System.out.println("Peer numPort :" + peer.numPort + "\n");
		System.out.println("Peer IP :" + peer.ipAddr + "\n");
		}
		catch(IOException e){}
		
	}

	public void startTreatment ()
	{	
		this.openConnection();
		String intersted = Treatments.interestedTreatment(key);
		//sendToServer(Treatments.interestedTreatment(key));
		sendToServer(intersted);
		System.out.println ("INTERESTED " + intersted + "\n");
		String haveResp = receiveFromServer();
		System.out.println ("HAVE RESP :" + haveResp );
		String buff= haveResp.substring(38,haveResp.length());
		//byte [] buffBytes = buffString.getBytes();
		//buff = new BufferMap(haveResp.substring(38,haveResp.length()));
		//buff = new BufferMapBis(buffBytes);
		//getOffsets();
		//getOffsets(receiveFromServer());  					// recuperer les offsets (en prend en compte les pieces qu'on a deja)
		sendToServer(Treatments.getPiecesTreatment (key, offsets));
		//int nbPieces = Bdd.FILES_TRACKER.get(key).nbPieces;
		//sendToServer(Treatments.getPiecesTreatment(key, nbPieces));
		Parser.treatResp(receiveFromServer());
			// 	on telecharge les pieces 
				
	}

	public static ArrayList<Integer> charToOffsets(char c){
		ArrayList<Integer> offsets = new ArrayList<Integer>();
		int x;
		int cc = (int) c;
		for(int i=0; i<8; i++){
			x = 1<<i;
			if ((cc & x) != 0)
				offsets.add(i);
		}
		return offsets;
	}
	public ArrayList<Integer> updateBufferMap () {
		int l = offsets.size();
		for (int i=0; i<l; i++){
			if (!currentOffsets.contains(offsets.get(i)))
				currentOffsets.add(offsets.get(i));
		}
		return currentOffsets;
	}
	private void addToOffsets(ArrayList<Integer> os, int n){
		int size = os.size();
		for(int i=0; i<size-1; i++){
			if(currentOffsets.contains(os.get(i)+8*n))
				offsets.add(os.get(i)+8*n);
		}
	}
	// private void getOffsets(String haveResp){
	// 	buff = new BufferMap(haveResp.substring(38,haveResp.length()));

	// 	// int nbChar = charBuff.length();
	// 	// for (int i=0;i<nbChar;i++){
	// 	// 	System.out.println(charBuff.charAt(i) + "over here \n");
	// 	// 	addToOffsets(charToOffsets(charBuff.charAt(i)), i);
	// 	// }

	// }
	// public void getOffsets () {
	// 	int nb = buff.getPiecesNumber();
	// 	for (int i=1; i<nb+1; i++){
	// 		if (buff.getValue(i) && currentOffsets.contains(i))
	// 			offsets.add(i);
	// 	}
	// }
	private void downloadPieces (String dataResp){
		;
	}
	public void updateBufferMap(byte[] buff){
		;
	}
	private void getBufferMap(String haveResp){
		;
	}
	// public static void main (String[] args)
 // 	{
 // 		ArrayList<Integer> off = new ArrayList<Integer> ();
 // 		off.add(0);
 // 		off.add(7);
 // 		ClientComm cc = new ClientComm(off);
 // 		String haveResp = "have 11111111111111111111111111111111 AA";
 // 		cc.getOffsets(haveResp);
 // 		System.out.println(cc.offsets.get(3)+"\n");
 // 	}
}
