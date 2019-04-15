using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AdjustBrushPosition : MonoBehaviour {

    private float desiredAngle = 0;
    private bool requestChange = false;

	// Use this for initialization
	void Start () {
      
        Debug.Log("Bonjour!  My initial position is " + transform.localEulerAngles.x + ", " + transform.localEulerAngles.y + ", " + transform.localEulerAngles.z);
       
    }
	
	// Update is called once per frame
	void Update () {

        GameObject go = GameObject.Find("Main Camera");
        MainSceneScript mainUdinoManager = go.GetComponent<MainSceneScript>();
        float angle = mainUdinoManager.BrushAngle;

        if (Input.GetKeyDown(KeyCode.A))
        {
            desiredAngle += 10.0f;
            requestChange = true;           
        }
        if (Input.GetKeyDown(KeyCode.Q))
        {
            desiredAngle -= 10.0f;
            requestChange = true;
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            desiredAngle = 0f;
            requestChange = true;        
        }


        if (requestChange)
        {
            requestChange = false;
            desiredAngle = AngleZeroTo360(desiredAngle);

         
            transform.localEulerAngles = new Vector3(desiredAngle, transform.localEulerAngles.y, transform.localEulerAngles.z);

        }




    }

    private float AngleZeroTo360(float inputAngle)
    {
        float temp = inputAngle;
        while (temp < 0)
            temp += 360;

        while (temp >= 360)
            temp -= 360;

        return temp;
    }
}
