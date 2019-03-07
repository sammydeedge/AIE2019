using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class cannon : MonoBehaviour {

    public GameObject prfb_CannonBall = null;
    public Transform spawnPoint = null;
    public float forceOnFire = 1200;
    public float cooldownTime = 0.2f;

    float cooldownTimer = 0;
	
	// Update is called once per frame
	void Update () {
		if(Input.anyKeyDown && cooldownTimer <= 0)
        {
            GameObject GO = Instantiate(prfb_CannonBall, spawnPoint.position, spawnPoint.rotation);
            if (GO == null) { return; }

            Rigidbody RB = GO.GetComponent<Rigidbody>();
            if (RB == null) { return; }

            RB.AddForce(GO.transform.forward * forceOnFire);
            cooldownTimer = cooldownTime;
        }
        if(cooldownTimer > 0)
        {
            cooldownTimer -= Time.deltaTime;
        }
	}
}
