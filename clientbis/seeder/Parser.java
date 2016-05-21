import java.net.ServerSocket;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.io.*;

public class Parser
{
	//public static HashMap<String, String[]> TAB; //= new HashMap<String, String[] >();
	public static String[] tab;
	// public static Vector<Integer> BufferMap(byte[] buffer)

	// {
	// 	Vector<Integer> vect = new Vector<Integer>();
		
	// 	for(int i=0; i<buffer.length; i++)
	// 		for(int bit=0; bit<8; bit++)
	// 			if((buffer[i] & (1 << bit)) != 0)
	// 				vect.add(i*8+bit);
				
	// 	return vect;
	// }



	public static void treatResp(String str)
	{
		if (str=="")
			System.out.println("treatResp problem\n");
		String[] parts = str.split(" ");
		if (parts[0].equals("list"))
			if (str.substring(6).equals("]") || str.substring(5,8).equals("[ ]"))
				return;
			else 
				{
					treatLook(str.substring(6, str.length()-1)); //6 = start index (and length-1 cuz of [])
					FilePeer.isPresent = true;
				}
			
		if (parts[0].equals("peers"))
			treatGetFile(str.substring(6,38),str.substring(40,str.length()-1));

		if (parts[0].equals("have"))
			treatInterested(str.substring(5,37),str.substring(38,str.length()));
		if (parts[0].equals("interested"))
			treatRequestLeecher(str.substring(11,str.length()));
		if (parts[0].equals("data"))
			treatData(str.substring(5,37), str.substring(39,str.length()-1));
	}




	public static void treatLook(String str)
	{
		int i=0;
		String tab[] = str.split(" ");

		while(i < tab.length)
		{
			Bdd.FILES_TRACKER.put(tab[i+3], new FilePeer(tab[i], 
                                Long.parseLong(tab[i+1]), 
                                Integer.parseInt(tab[i+2]), tab[i+3]));
			i += 4;
		}
	}
	

public static void treatGetFile(String key, String str)
	{
		ArrayList<Peer> li = new ArrayList<Peer>();
		if(str.equals(""))
			return;
			
		String tab[] = str.split(" ");
		
		for(int i=0; i < tab.length; i++)
		{
			
			Peer p = new Peer(tab[i]);
			li.add(p);
			
			//Leecher l = new Leecher(key, p);
			//l.start();
		}
		Bdd.KEY_TO_PEER.put(key, li);


	 	FilePeer fp = Bdd.FILES_TRACKER.get(key);
	 	double x = (double)fp.size /(double)fp.sizePieces;
	 	int nbOffsets =  (int) Math.ceil(x);
	 	for (int j=0; j<nbOffsets; j++)
	 		{
				PieceSize ps = new PieceSize(key,j);
				Bdd.PIECE_TO_PEERS.put(ps, new ArrayList<Peer>());	 		
	 		}

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



public static void treatInterested(String key, String buff)
	{
		String ip = "1.1.1.1";
		int port = 777777;
		Peer p = new Peer(ip,port);	// à instancier avec les bons attributs
		int off=0;
		for (int j=0; j<buff.length(); j++)
			{

				ArrayList<Integer> os = new ArrayList<Integer>();
				os = charToOffsets(buff.charAt(j));
				for (int k=0; k<os.size(); k++)
				{
					PieceSize ps = new PieceSize(key,os.get(k)+off);
					Bdd.PIECE_TO_PEERS.get(ps).add(p);
				}		
			off = off + 8;
			}

	}

	public static void treatRequestLeecher(String key)
	{
		FilePeer fp= new FilePeer(key);
		Bdd.FILES_LEECH.put(key,fp);
	}


	public static void treatData(String key, String indPiece)
	{
		int i = 0;
		/*File f = new File(Config.DIR_SEED+"/"+Bdd.FILES_SEED.get(this.key).filename;
		 
		 if (!file.exists())
		 {
		 	f.createNewFile();
		 }

		*/
		String listIndPiece[] = indPiece.split(" "); // 5:p5 7:p7 
		while (i < listIndPiece.length)
			{
				String b[] = listIndPiece[i].split(":");
				tab[Integer.parseInt(b[0])] = b[1];
				i++;
			}
	}


	public void writeFusionFiles(String destFile) throws IOException { //destfile : ./FILES/SED/a.png
    // On ouvre (ou on crée) le fichier de destination :
    OutputStream out = new FileOutputStream(destFile);
    try {

      for (int i = 0; i < tab.length; i++) {
        //int off = Integer.toString(i).length() + 1;
        //if(i%10 == 9)
         // off++;
        //String target = sourceFiles[i].substring(off, sourceFiles[i].length());
        byte[] buf = tab[i].getBytes();
        int len = tab[i].length();
        try {
          // On lit dans le buffer
            out.write(buf, 0, len); // et on copie ce qu'on a lu
        } finally {
        }
      }

    } finally {
      out.close();
    }
  }



public static void config()
	{
		BufferedReader fread = null;
		String line = "";
                
		try {
			fread = new BufferedReader(new FileReader(new File(Config.FILE_CONF)));
		}catch(IOException e){}
                
                if (fread == null) {
                    try {
                        fread = new BufferedReader(new FileReader(new File("client\\" + Config.FILE_CONF)));
                    } catch (IOException e) {}
                }
                
		int i=0;
		do 
		{
			try
			{
				line = fread.readLine();
			
				if (line != null && !(line.equals("")) && !(line.substring(0,1).equals("%")))
				{
					switch(i)
					{
						case 0:
							Config.SERVER_IP = line;
							i++;
							break;
						
						case 1:
							Config.SERVER_PORT = Integer.parseInt(line);
							i++;
							break;
							
						case 2:
							Config.MAX_PEER = Integer.parseInt(line);
							i++;
							break;
						
						case 3:
							Config.INTERVAL_MAJ = Integer.parseInt(line);
							i++;
							break;
						
						case 4:
							Config.SIZE_PIECES = Integer.parseInt(line);
							i++;
							break;
						case 5:
							Config.DIR_SEED = line;
							i++;
							break;
					}
				}
			}catch(IOException e){}
				
		}while(line != null && i<3);
		
		try
		{
			fread.close();
		}catch(IOException e){}
	}




	public static String MD5(String s)
	{
		try
		{
		
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(s.getBytes());
			byte[] md5_b = md.digest();
			
			StringBuilder md5 = new StringBuilder(2*md5_b.length);
			byte[] Hexhars = {
				'0', '1', '2', '3', '4', '5',
				'6', '7', '8', '9', 'a', 'b',
				'c', 'd', 'e', 'f' 
			};
			
			for (int i = 0; i < md5_b.length; i++) 
			{
				int v = md5_b[i] & 0xff;
				md5.append((char)Hexhars[v >> 4]);
				md5.append((char)Hexhars[v & 0xf]);
			}

			return md5.toString();
			
		}
		catch(NoSuchAlgorithmException e){
			return null;
		}
	}
	




}
