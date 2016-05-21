import java.io.File;
import java.util.HashMap;
import java.util.*;
import java.io.*;

public class Bdd
{
	public static HashMap<String, FilePeer> FILES_SEED; //correspondance clé et Filepeer
	public static HashMap<String, FilePeer> FILES_LEECH; //correspondance clé et Filepeer
	public static HashMap<String, FilePeer> FILES_TRACKER; //fichiers dispo dans le tracker après des look
	public static HashMap<String, FilePeer> FILES_USER;// correspondance filename et file
	public static HashMap<String, ArrayList<Peer>> KEY_TO_PEER; //correspondance entre clé et peers
	public static HashMap<PieceSize, ArrayList<Peer>> PIECE_TO_PEERS; //correspondance entre clé et peer


	synchronized static public ArrayList<Peer> getKeyToPeer(String key){
		return KEY_TO_PEER.get(key);	
	}

	synchronized static public void putFilesUser(String fileName, FilePeer file){
		//synchronized(this.FILES_USER){
			FILES_USER.put(fileName, file);
		//}
		return;
	}

	synchronized static public Set<String> keySetFilesUser() {
		//synchronized(this.FILES_USER){
			return FILES_USER.keySet();
		//}
	}

	// synchronized static public void putFilesUser(String fileName, FilePeer file){
	// 	FILES_USER.put(fileName, file);
	// }
	synchronized static public boolean isEmptyFilesUser(){
		return FILES_USER.isEmpty();
	}
	public static void bddUser()
		{
 			final Enumeration<String> strEnum = Collections.enumeration(FILES_USER.keySet());
 			while(strEnum.hasMoreElements()) 
 			{
 				String key = strEnum.nextElement();
 			}
 		}

	public static void bddTrack()
		{
 			final Enumeration<String> strEnum = Collections.enumeration(FILES_TRACKER.keySet());
 			while(strEnum.hasMoreElements()) 
 			{
 				String key = strEnum.nextElement();
 			}
 		}

	public static void bddSeed()
		{
 			final Enumeration<String> strEnum = Collections.enumeration(FILES_SEED.keySet());
 			while(strEnum.hasMoreElements()) 
 			{
 				String key = strEnum.nextElement();
 				System.out.println("bdd seed debug \n");
 				System.out.println("key : " + key + "filename : " + FILES_SEED.get(key).filename); 
 			}
 		}

/*	public static void bddPeer2()
		{
			int i=0;
 			final Enumeration<String> strEnum = Collections.enumeration(PIECE_TO_PEERS.keySet());
 			//System.out.println("la taille est " + KEY_TO_PEER.size() +"\n");
 			while(strEnum.hasMoreElements()) 
 			{
 				PieceSize ps = strEnum.nextElement();
	     		System.out.println("KEY :"+ps.key+ " OFFSET" + ps.offset + "\nLISTE DES ADDR IP : \n");
	     		while (i < PIECE_TO_PEERS.get(ps).size())
		  	 	  	{
		  	 	  		System.out.println("Element n°"+i + " : " + PIECE_TO_PEERS.get(ps).get(i).ipAddr +"\n");
		  	 	  		i++;
					}
 			}
 		}
*/
 		public static void bddPeer()
		{
			int i=0;
 			final Enumeration<String> strEnum = Collections.enumeration(KEY_TO_PEER.keySet());
 			//System.out.println("la taille est " + KEY_TO_PEER.size() +"\n");
 			while(strEnum.hasMoreElements()) 
 			{
 				String key = strEnum.nextElement();
	     		System.out.println("KEY :"+key+"\nLISTE DES ADDR IP : \n");
	     		while (i < KEY_TO_PEER.get(key).size())
		  	 	  	{
					    System.out.println("Element n°"+i + " : " + KEY_TO_PEER.get(key).get(i).ipAddr +"\n");
					    i++;
					}
 			}
 		}






	public static void init()
	{
		// Liste le répertoire seed
		FILES_SEED = new HashMap<String, FilePeer>();
		File repertoire = new File(Config.DIR_SEED);
                if (! repertoire.exists()) {
                	System.out.println("DIRECTORY DOES NOT EXIST\n");
                    //repertoire = new File("client\\" + Config.DIR_SEED);
                }
		String[] children = repertoire.list(); 
		
		for(int i=0; i<children.length; i++)
		{
			String fileName = children[i];
			
			if(!(fileName.equals(".") || fileName.equals("..")))
			{
				String hash = Parser.MD5(fileName);
				if(fileName.equals("a.txt"))
					System.out.println("a.txt key : "  + hash);
				FilePeer fp = new FilePeer(fileName, (new File(Config.DIR_SEED+"/"+fileName)).length(), Config.SIZE_PIECES, hash);
				fp.fillBufferMap();
				//Arrays.fill(fp.buffMap, (byte)0xff);
				FILES_SEED.put(hash, fp);
			}
		}

		
		
		// Liste le répertoire leech
		FILES_LEECH = new HashMap<String, FilePeer>();
		
		// Liste des fichier dispo sur le tracker (après des look)
		FILES_TRACKER = new HashMap<String, FilePeer>();
		
		//A un filemname correspond son FilePeer
		FILES_USER= new  HashMap<String, FilePeer>();
		// A une clé correspond la liste des peer
		KEY_TO_PEER = new  HashMap<String, ArrayList<Peer>>();
		
		PIECE_TO_PEERS = new HashMap<PieceSize, ArrayList<Peer>>(); 
	


	}

}
