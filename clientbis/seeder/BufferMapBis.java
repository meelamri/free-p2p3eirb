import java.io.*;
import java.net.*;
import java.lang.String;
import java.util.*;

public class BufferMapBis {

ArrayList<Integer> offsets;
byte [] buff;
public BufferMapBis(byte [] b) {
	this.buff = b;
	return;
}


private static Boolean stateBit(byte b, int bit)
	{
		return (b & (1 << bit)) != 0;
	}


	public static ArrayList<Integer> getIndexFromBufferMap(byte[] buf)
	{
		ArrayList<Integer> v = new ArrayList<Integer>();
		
		for(int i=0; i<buf.length; i++)
			for(int b=0; b<8; b++)
				if(stateBit(buf[i], b))
					v.add(i*8+b);
				
		return v;
	}


}