package com.example.tcpAesCrc;


import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;

import com.example.aes128.AES128;
import com.example.crc32.crc32;

/*--------TCP/SERVER/AES128/CRC32---------
 * 
 * Name: 			HECTOR CARLOS DE JESUS FLORES FLORES
 * e-mail:			carlos.flogars@gmail.com
 * Date:			12/February/2019
 * 
 * Description: This module test a tcp server, using the 
 * standard encryption of AES128 and the checksum CRC32.
 * Basically there is two functions at the bottom that received
 * and sent messages with AES128 and CRC32 you only need to pass
 * them the message and the key and they will do all the work.
 * 
 *---------------------------------------- */

public class TestServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub

		/* Lets create a socket */
		ServerSocket SerSocket = new ServerSocket(50002);

		/* Print listening mode at port XXXX */
		System.out.println("Start listening at socket: 50002");

		while (true) {

			/* Print waiting for client */
			System.out.println("Waiting for client....");

			/* Waiting for client to accept de connection */
			Socket mySocket = SerSocket.accept();

			/* Create the input of the socket */
			InputStream in = mySocket.getInputStream();

			/* Create the output of the socket */
			OutputStream out = mySocket.getOutputStream();

			/* Print to wait for the info that the client will send us */
			System.out.println("Waiting for the client info...");

			/* Create a byte array for receiving the bytes */
			byte messageByte[] = new byte[26];

			/* Read the client port */
			in.read(messageByte);

			/*Function that verify crc and decrypt message*/
			aesCrcReceive(messageByte,"1111111111111111");			

			/* Write in the socket */
			out.write(aesCrcSend("Te respondi.....", "1111111111111111"));

			/* Print response sent to know when the message was sent */
			System.out.println("Response sent");

			/* Close socket */
			mySocket.close();

		}
	}

	private static byte[] aesCrcSend(String stringAES, String key) throws UnsupportedEncodingException, Exception {

		/* Encrypting the message of 16 bytes */
		byte a[] = AES128.ecbEncrypt(stringAES.getBytes("UTF-8"), key);

		/* Getting the crc of the encrypted message */
		byte b[] = Long.toString(crc32.getCrc32(AES128.ecbEncrypt(stringAES.getBytes("UTF-8"), key)))
				.getBytes();

		/* Creating the new byte array to save the encrypted message with the crc */
		byte[] c = new byte[a.length + b.length];

		/* copy byte array a to c */
		System.arraycopy(a, 0, c, 0, a.length);

		/* copy byte array b to c */
		System.arraycopy(b, 0, c, a.length, b.length);

		return c;
	}

	private static void aesCrcReceive(byte[] data, String key) throws UnsupportedEncodingException, Exception {

		/* Create byte array to store the message encrypted received from the client */
		byte[] messageByte = Arrays.copyOfRange(data, 0, 16);

		/* Create byte array to store the crcChecksum received from the client */
		byte[] crcByte = Arrays.copyOfRange(data, 16, 25);

		/* Convert from byte array to string */
		String crcString = new String(crcByte, "UTF-8");

		/* Check the message calculating the crc32 */
		String messageString = Long.toString(crc32.getCrc32(messageByte));

		/*Cut the message to take just 9 bytes*/
		messageString = messageString.substring(0, 9);

		/* Compare two crc, the calculated and the received from the client */
		if (0 == messageString.compareTo(crcString)) {
			System.out.println("The message received arrived COMPLETE!!!");
		} else {
			System.out.println("The message received ARRIVED INCOMPLETE!!!");
		}

		/* Decripting the message and pass it to an arrray */
		byte[] decryptedCipherText = AES128.ecbDecrypt(messageByte, key);

		/* Conver the message from bytes to String */
		String strResponse = new String(decryptedCipherText).trim();

		/* Conver the message from bytes to String */
		String information = new String(messageByte).trim();

		/* Print message received */
		System.out.println("The message received with encryption is: " + information);

		/* Print message received */
		System.out.println("The message received without encryption is: " + strResponse);


	}

}























