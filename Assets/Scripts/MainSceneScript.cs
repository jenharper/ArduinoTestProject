using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Uduino;

public class MainSceneScript : MonoBehaviour {

    private int counter = 0;
    public UduinoManager m_uduinoController;

    void Awake()
    {
        m_uduinoController.OnDataReceived += OnDataReceived; //Create the Delegate
        m_uduinoController.alwaysRead = true; // This value should be On By Default
    }

    // Use this for initialization
    void Start () {
        Debug.Log("Bonjour!  MainSceneScript is alive.");
      //  m_uduinoController.DiscoverPorts();
    }

    //May want to move this to statemanage
    void OnDataReceived(string data, Uduino.UduinoDevice deviceName)
    {
            Debug.Log(deviceName.lastRead  + " aka " + deviceName.identity + " said " + data);   
    }

    // Update is called once per frame
    void Update () {
        counter++;
        if (counter > 500)
        {
            counter = 0;
            Debug.Log("MainSceneScript heartbeat");
        }
	}
}
