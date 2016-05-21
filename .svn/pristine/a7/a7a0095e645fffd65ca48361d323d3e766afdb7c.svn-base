import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;

public class Leech implements Runnable{

	private String key;
	private FilePeer fp;
	private ArrayList<Integer> ourOffsets;
	private	Thread thread;
	private ArrayList<Peer> peers;
	private HashMap<Peer, byte[]> PEER_BUFFERMAP;
	int nbPieces;
	ClientComm clientComm;
	public Leech(String key){
		peers = Bdd.getKeyToPeer(key);
		ourOffsets = new ArrayList<Integer> ();
		this.key = key;
		thread = new Thread(this);
		thread.start();
	}
	public void run () {
		int nbPeers = peers.size();
		int i=0;
		// on traite les peers un à un
		for (i=0;i<nbPeers;i++){
			clientComm = new ClientComm(key, peers.get(i), ourOffsets);
			clientComm.startTreatment();
			ourOffsets = clientComm.updateBufferMap();
			//getBufferMap(peers.get(i));      // remplit la table (peer, buffmap)
		}
		//for (i=0;i<nbPieces;i++){
		//	CommClient(i);
		//}
	}	
	// private void getBufferMap(Peer p){
	// 	Treatments.interestedTreatment(key);
	// 	Parser.treatResp();
	// }
	// private void getPiece(int offSet){
	// 	;
	// }

}