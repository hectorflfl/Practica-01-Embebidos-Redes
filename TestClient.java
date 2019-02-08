package com.example.aes128;


import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import com.example.crc32.crc32;



public class TestClient {

	public static void main(String[] args) throws Exception {
		
		byte a[] = AES128.ecbEncrypt("HOLA MUNDO CRUEL".getBytes("UTF-8"),"1111111111111111");
		
		
		byte b[] = Long.toString(crc32.getCrc32(AES128.ecbEncrypt("HOLA MUNDO CRUEL".getBytes("UTF-8"),"1111111111111111"))).getBytes();
		byte[] c = new byte[a.length + b.length];
		System.arraycopy(a, 0, c, 0, a.length);
		System.arraycopy(b, 0, c, a.length, b.length);
		
		Socket sock = new Socket("192.168.0.101",50001);
		System.out.println("Connected to the server");
		
		
		
		//Create the input and out of the socket
		InputStream in = sock.getInputStream();
		OutputStream out = sock.getOutputStream();
		
		System.out.println("Sending product info...");
		
		out.write("Hola mundo".getBytes());
		
		byte response[] = new byte[16];
		in.read(response);
		
		
		String s=new String(response);

		System.out.println(s);
		byte[] decryptedCipherText = AES128.ecbDecrypt(response,"1111111111111111");
		String strResponse = new String(decryptedCipherText).trim();
		
		System.out.println("Obtained response is: " + strResponse);
		
		sock.close();
		
	}

}
