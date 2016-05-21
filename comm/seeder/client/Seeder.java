import java.io.*;
import java.net.Socket;


import java.io.IOException;
import java.net.*;



public class Seeder extends Thread
{
	public Socket sock;
	public String key;
	public byte[] dataB;
	public OutputStream out;


	public Seeder(Socket s)
	{
		super();
		System.out.println("HERE IN THE Seeder COSNRT");
		this.sock = s;
		System.out.println("Leecher ip : " + sock.getLocalAddress());
	}


	public void run()  
	{
		
		try
		{
			out = this.sock.getOutputStream();
			// Recepetion de "interested $key"
			System.out.println("Before Leecher response :" +  "\n");
			String str = this.receiveFromLeecher();
			System.out.println("Leecher response :" +str + "\n");
			this.key = str.substring(11, str.length());
			String haveandkey = "have "+this.key+" ";
		
			// Envoi de "have $key $bufferMap"
			//out.write(haveandkey.getBytes());
			//out.write(Bdd.FILES_SEED.get(this.key).buffMap);
			//out.write("\n".getBytes());

			out.write(haveandkey.getBytes());
			System.out.println("key : " + this.key + "\n");
			out.write(Bdd.FILES_SEED.get(this.key).buffMap);
			Bdd.bddSeed();
			out.write("\n".getBytes());
			this.sendToLeecherByte();
			
			
			// Reception de "getpieces $key [i1 ... in]"
			String str2 = this.receiveFromLeecher();
			String list_index_str = str2.substring(44, str2.length()-1); 
			String list_index[] = list_index_str.split(" ");
			
			// Envoi de "data $key [i1:p1 ...]"
			String dataSend = "data "+this.key+" [";
			out.write(dataSend.getBytes());
			File f = new File(Config.DIR_SEED+"/"+Bdd.FILES_SEED.get(this.key).filename);
			this.dataB = this.extractBytes(f);
			this.reconstruction(list_index);
			this.sendToLeecherByte();
			
		}
		catch (IOException ex) { }
	}
	



	// private String receiveFromLeecher()
	// {
	// 	if(this.sock == null || !this.sock.isConnected())
	// 	{
	// 		System.out.println("Socket not connected.");
	// 		return null;
	// 	}

	// 	try{

	// 		BufferedReader receive_s = new BufferedReader(new InputStreamReader(this.sock.getInputStream()));
	// 		String str = receive_s.readLine();
			
	// 		//Tools.traceFluxReceive("seeder : "+str);
	// 		return str;
	// 	}catch(IOException e){ return null; }
	// }
	
	private String receiveFromLeecher()
	{
		if(this.sock == null || !this.sock.isConnected())
		{
			System.out.println("Socket not connected.");
			return null;
		}

		try{

			BufferedReader receive_s = new BufferedReader(new InputStreamReader(this.sock.getInputStream()));
			//String str = receive_s.readLine();
			int c;
			String str = "";
			for(int i=0; i<43; i++) {
				c = receive_s.read();
				str = str + (char) (c);
				//System.out.println(str + "\n");
				//receive_s.skip(1);
		}
			
			//Tools.traceFluxReceive("seeder : "+str);
			return str;
		}catch(IOException e){ return null; }
	}
	



	private void sendToLeecherByte()
	{
		if(this.sock == null || !this.sock.isConnected())
		{
			System.out.println("Socket not connected.");
			return;
		}

		
		try{
			System.out.println(" before flushing \n");
			this.out.flush();
		}
		catch (IOException ex) { }
	}
	// private void sendToLeecherByte(String s)
	// {
	// 	if(this.sock == null || !this.sock.isConnected())
	// 	{
	// 		System.out.println("Socket not connected.");
	// 		return;
	// 	}

	// 	//Tools.traceFluxSend("seeder : Bytes");
		
	// 	try{
	// 		this.print(s);
	// 		this.out.flush();
	// 	}
	// 	catch (IOException ex) { }
	// }



	
	private void reconstruction(String list_index[]) throws IOException
	{
		for(int i=0; i<list_index.length-1; i++)
			this.partialReconstruction(list_index[i], false);
		this.partialReconstruction(list_index[list_index.length-1], true);
		
		out.write("]\n".getBytes());
	}
	



	
	private void partialReconstruction(String id, Boolean last) throws IOException
	{
		out.write(id.getBytes());
		out.write(":".getBytes());
		
		byte[] bytes = new byte[Bdd.FILES_TRACKER.get(this.key).getSizePieces()];

		int start = bytes.length*Integer.parseInt(id);
		int end = start+bytes.length;
		for(int i = start; i<end; i++)
			bytes[i-start] = this.dataB[i];

		
		out.write(bytes);
		
		if(!last)
			out.write(" ".getBytes());
	}



	// retourne un tableau de 

	public byte[] extractBytes(File file) throws IOException 
	{
		InputStream is = new FileInputStream(file);

		// Get the size of the file
		long length = file.length();

		// You cannot create an array using a long type.
		// It needs to be an int type.
		// Before converting to an int type, check
		// to ensure that file is not larger than Integer.MAX_VALUE.
		if (length > Integer.MAX_VALUE) {
		    // File is too large
		}

		// Create the byte array to hold the data
		byte[] bytes = new byte[(int)length];

		// Read in the bytes
		int offset = 0;
		int numRead = 0;
		while (offset < bytes.length
		       && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
		    offset += numRead;
		}

		// Ensure all the bytes have been read in
		if (offset < bytes.length) {
		    throw new IOException("Could not completely read file "+file.getName());
		}

		// Close the input stream and return bytes
		is.close();
		return bytes;
	}



}