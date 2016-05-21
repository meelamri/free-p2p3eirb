import java.util.Arrays;

public class FilePeer
{

	public String filename;
	public long size;
	public int sizePieces;
	public int nbPieces;
	public String key;
	//public 
	public byte[] buffMap;
	public static boolean isPresent; //si le tracker nous a repondu quel client possede tel fichier
	//private boolean
	
	public FilePeer(String filename, long size, int sizePieces, String key)
	{
		this.filename = filename;
		this.size = size;
		this.sizePieces = sizePieces;
		this.key = key;
		
		//this.buffMap = new byte[this.buffMapSize()];
		this.isPresent = false;
		//this.nbPieces = Math.ceil((double)size/(double)sizePieces);
		//Arrays.fill(this.buffMap, (byte)0x0);
	}

	public	FilePeer(String filename)
	{
		this.filename = filename;
		//this.isPresent = false;

	}

	// public FilePeer(String key)
	// {
	// 	this.key = key;
	// }

	public String display()
	{
		return filename+" "+size+" "+sizePieces+" "+key;
	}
	
	
	public void refreshBuffer(int position)
	{
		this.buffMap[(int)(position/8)] |= (1 << (7-position));
	}
	
	
	public void fillBufferMap()
	{
		Arrays.fill(this.buffMap, (byte)0xff);
	}
	
	
	public byte[] getBufferMap()
	{
		return this.buffMap;
	}
	
	
	public int buffMapSize()
	{
		return (int)(Math.ceil((this.size/this.sizePieces)/8));
	}
	
	
	public String getName()
	{
		return this.filename;
	}
	
	
	public int getSizePieces()
	{
		return this.sizePieces;
	}
	
	public long getSize()
	{
		return this.size;
	}
	
	public int getNbPieces()
	{
		return (int)(Math.ceil((this.size/this.sizePieces)));
	}
	
	public int sizeLastPiece()
	{
		return (int)(this.size - (this.sizePieces*(this.getNbPieces()-1)));
	}
	
}