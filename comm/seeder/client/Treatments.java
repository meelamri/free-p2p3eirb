import java.util.*;


public class Treatments {
	// public void listTreatment(String str){
	//     	String[] fragments = str.split(" ");
	//     	if (! fragments[0].equals("list")){
	//     		System.out.println("expected list protocol but not received");
	//     		System.exit(0);
	//     	}
	//     	return;
	//     }
		public static String announceTreatment(){
			String keyTable;
			FilePeer file;
			String seed;
			String request = "announce listen" + " " + Config.SERVER_PORT + " seed [";
			Enumeration<String> strEnum = Collections.enumeration(Bdd.FILES_SEED.keySet());
 			while(strEnum.hasMoreElements()) 
 				{
 					keyTable = strEnum.nextElement();
 					file = Bdd.FILES_SEED.get(keyTable);
 					seed =  file.filename + " " + Long.toString(file.size) + " " + Integer.toString(file.sizePieces) + " " + keyTable + " ";
 					request = request + seed;
 				}
			request = request.substring(0,request.length()-1) + "] leech []";
			return request;
		}
		public static String lookTreatment(String str){
			//System.out.println("lookTreatmentTest\n");
			if (str == null){
				System.out.println("lookTreatment error");
				System.exit(0);
			}
			//String request = "";
			//request.concat("look [filename =" + str + "]") ;
			String request = "look [filename=\"" + str + "\"]";
			//System.out.println(request);
			
			return request;
		}
		public static String getFileTreatment(String key){
			//System.out.println("lookTreatmentTest\n");
			if (key == null){
				System.out.println("lookTreatment error");
				System.exit(0);
			}
			//String request = "";
			//request.concat("look [filename =" + str + "]") ;
			String request = "getfile " + key;
			//System.out.println(request);
			
			return request; 	
		}
		public static String interestedTreatment(String key){
			String request = "interested " + key;
			return request;

		}
		public static String haveTreatment(String key, String bufferMap)
		{
			String resp = "have " + key + " " + bufferMap;
			return resp;
		}

		public static String getPiecesTreatment (String key, ArrayList<Integer> index)
		{
			String request = "getpieces "+key+" ";
			int i=0;
			int l = index.size();
			while (i<l)
			{
				request = request + index.get(i).toString();
				i++;
			}
			return request;
		}
		
		public static String updateTreatment(ArrayList<String> seed, ArrayList<String> leech)
		{
			String request = "update seed [";
			int i=0;
			int l1 = seed.size();
			int l2 = leech.size();
			while (i<l1)
			{
				request = request + seed.get(i);
				i++;
			}
			request = request + "] leech [";
			i = 0;
			while (i<l1)
			{
				request = request + leech.get(i);
				i++;
			}
			request = request + "]";
			return request;
		}





}
