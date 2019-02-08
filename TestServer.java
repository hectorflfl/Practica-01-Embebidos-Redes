package com.example.aes128;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;

import com.example.crc32.crc32;


public class TestServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub

	
		/*Lets create a socket*/
		ServerSocket SerSocket = new ServerSocket(50002);
		System.out.println("Start listening at socket: 50002");
		
		while(true)
		{
			
			System.out.println("Waiting for client....");
			Socket mySocket = SerSocket.accept();
			
			/*Create the input and out of the socket*/
			InputStream in = mySocket.getInputStream();
			OutputStream out = mySocket.getOutputStream();
			
			System.out.println("Waiting for the client info...");
			byte data[] = new byte[26];
			in.read(data);
			
			
			byte[] messageByte = Arrays.copyOfRange(data, 0, 16);
			byte[] crcByte = Arrays.copyOfRange(data, 16, 26);
			String messageString = Long.toString(crc32.getCrc32(messageByte));
			String crcString = new String(crcByte, "UTF-8");

			

			
			if (0 == messageString.compareTo(crcString)) {
				System.out.println("The message received is correct!!!");
			}else {
				System.out.println("The message received is NO correct!!!");
			}
			
			
			
			byte[] decryptedCipherText = AES128.ecbDecrypt(messageByte,"1111111111111111");
			String strResponse = new String(decryptedCipherText).trim();

			String information = new String(messageByte).trim();
			
			
			System.out.println("Received the product name : " + information);
			System.out.println("Received the product name : " + strResponse);
			
			
		
			out.write("Te respondi...".getBytes());

			System.out.println("Response sent");

			
			mySocket.close();
			
		}
	}

}
