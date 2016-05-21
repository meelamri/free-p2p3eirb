

class Peer
{

	public String ipAddr;
	public int numPort;
	
	public Peer(String str)
	{
		String tab[] = str.split(":");
		this.ipAddr = tab[0];
		this.numPort = Integer.parseInt(tab[1]);
	}
	
	public Peer(String ip, int port){
		this.ipAddr = ip;
		this.numPort = port;
	}
	public String getIp()
	{
		return this.ipAddr;
	}
	
	
	public int getPort()
	{
		return this.numPort;
	} 
	
}
