using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AdjustBrushPosition : MonoBehaviour {

    float angleX = 0;

	// Use this for initialization
	void Start () {
      
        Debug.Log("Bonjour!  My initial position is " + transform.localEulerAngles.x + ", " + transform.localEulerAngles.y + ", " + transform.localEulerAngles.z);
        angleX = transform.localEulerAngles.x;
    }
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.A))
        {
            angleX += 10.0f;
            transform.Rotate(10, 0, 0, Space.Self);
            //transform.Rotation(10, 0, 0, Space.Self);
        }
        if (Input.GetKeyDown(KeyCode.Q))
        {
            angleX -= 10.0f;
            transform.Rotate(-10, 0, 0, Space.Self);
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            transform.Rotate(-1*angleX, 0, 0, Space.Self);
            angleX = 0f;
            //transform.eulerAngles = new Vector3(angleX, 0, 0);

        }
    }
}
