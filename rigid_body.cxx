/*!
  \file rigid_body.cxx
  \brief Auxiliary routines to solve equations of motion for rigid bodies
  \author J. Molina
  \date 2014/08/15
  \version 1.1
*/
#include "rigid_body.h"

void rigid_body_rotation(double             rotated[DIM],
                         const double       original[DIM],
                         const quaternion & q,
                         const COORD_TRANS &transform) {
    if (transform == BODY2SPACE) {
        qtn_qvq_prod(rotated, q, original);
    } else if (transform == SPACE2BODY) {
        quaternion qconj;
        qtn_conj(qconj, q);
        qtn_qvq_prod(rotated, qconj, original);
    } else {
        fprintf(stderr, "Error: unknown rigid body transformation\n");
        exit_job(EXIT_FAILURE);
    }
}

void rigid_body_matrix_rotation(double             rotated[DIM * DIM],
                                const double       original[DIM * DIM],
                                const quaternion & q,
                                const COORD_TRANS &transform) {
    quaternion dmy_q;
    if (transform == BODY2SPACE) {
        qtn_init(dmy_q, q);
    } else if (transform == SPACE2BODY) {
        qtn_conj(dmy_q, q);
    } else {
        fprintf(stderr, "Error: unknown rigid body transformation\n");
        exit_job(EXIT_FAILURE);
    }
    double A0[DIM] = {original[0], original[3], original[6]};
    double A1[DIM] = {original[1], original[4], original[7]};
    double A2[DIM] = {original[2], original[5], original[8]};
    qtn_qvq_prod(A0, dmy_q);
    qtn_qvq_prod(A1, dmy_q);
    qtn_qvq_prod(A2, dmy_q);

    rotated[0] = A0[0];
    rotated[1] = A1[0];
    rotated[2] = A2[0];
    rotated[3] = A0[1];
    rotated[4] = A1[1];
    rotated[5] = A2[1];
    rotated[6] = A0[2];
    rotated[7] = A1[2];
    rotated[8] = A2[2];

    qtn_qvq_prod(&rotated[0], dmy_q);
    qtn_qvq_prod(&rotated[3], dmy_q);
    qtn_qvq_prod(&rotated[6], dmy_q);
}

void rigid_body_rotation(double             rotated[DIM],
                         const double       original[DIM],
                         const double       QR[DIM][DIM],
                         const COORD_TRANS &transform) {
    if (transform == BODY2SPACE) {
        M_v_prod(rotated, QR, original);
    } else if (transform == SPACE2BODY) {
        v_M_prod(rotated, original, QR);
    } else {
        fprintf(stderr, "Error: unknown rigid body transformation\n");
        exit_job(EXIT_FAILURE);
    }
}
void rigid_body_rotation(double             rotated[DIM],
                         const double       original[DIM],
                         const double       QR[DIM * DIM],
                         const COORD_TRANS &transform) {
    if (transform == BODY2SPACE) {
        M_v_prod(rotated, QR, original);
    } else if (transform == SPACE2BODY) {
        v_M_prod(rotated, original, QR);
    } else {
        fprintf(stderr, "Error: unknown rigid body transformation\n");
        exit_job(EXIT_FAILURE);
    }
}

void rigid_body_matrix_rotation(double             rotated[DIM * DIM],
                                const double       original[DIM * DIM],
                                const double       R0[DIM * DIM],
                                const COORD_TRANS &transform) {
    if (transform == BODY2SPACE) {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                double dmy_k = 0.0;
                for (int k = 0; k < DIM; k++) {
                    double dmy_l = 0.0;
                    for (int l = 0; l < DIM; l++) {
                        dmy_l += original[k * DIM + l] * R0[j * DIM + l];
                    }
                    dmy_k += R0[i * DIM + k] * dmy_l;
                }
                rotated[i * DIM + j] = dmy_k;
            }
        }
    } else if (transform == SPACE2BODY) {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                double dmy_k = 0.0;
                for (int k = 0; k < DIM; k++) {
                    double dmy_l = 0.0;
                    for (int l = 0; l < DIM; l++) {
                        dmy_l += original[k * DIM + l] * R0[l * DIM + j];
                    }
                    dmy_k += R0[k * DIM + i] * dmy_l;
                }
                rotated[i * DIM + j] = dmy_k;
            }
        }
    }
}
void rigid_body_matrix_rotation(double             rotated[DIM][DIM],
                                const double       original[DIM][DIM],
                                const double       R0[DIM][DIM],
                                const COORD_TRANS &transform) {
    if (transform == BODY2SPACE) {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                double dmy_k = 0.0;
                for (int k = 0; k < DIM; k++) {
                    double dmy_l = 0.0;
                    for (int l = 0; l < DIM; l++) {
                        dmy_l += original[k][l] * R0[j][l];
                    }
                    dmy_k += R0[i][k] * dmy_l;
                }
                rotated[i][j] = dmy_k;
            }
        }
    } else if (transform == SPACE2BODY) {
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                double dmy_k = 0.0;
                for (int k = 0; k < DIM; k++) {
                    double dmy_l = 0.0;
                    for (int l = 0; l < DIM; l++) {
                        dmy_l += original[k][l] * R0[l][j];
                    }
                    dmy_k += R0[k][i] * dmy_l;
                }
                rotated[i][j] = dmy_k;
            }
        }
    }
}

void qdot(quaternion &dqdt, const quaternion &q, const double omega[DIM], const COORD_SYSTEM &coord) {
    quaternion qw;
    qtn_init(qw, 0.0, omega);  // omega in body or space coord
    if (coord == SPACE_FRAME) {
        qtn_prod(dqdt, qw, q, 0.5);
    } else if (coord == BODY_FRAME) {
        qtn_prod(dqdt, q, qw, 0.5);
    } else {
        fprintf(stderr, "Error: unknown coordinate system\n");
        exit_job(EXIT_FAILURE);
    }
}

void Qdot(double dQRdt[DIM][DIM], const double QR[DIM][DIM], const double omega[DIM], const COORD_SYSTEM &coord) {
    double omega_skew[DIM][DIM];
    skew(omega_skew, omega);  // omega in body or space coord
    if (coord == SPACE_FRAME) {
        M_prod(dQRdt, omega_skew, QR);
    } else if (coord == BODY_FRAME) {
        M_prod(dQRdt, QR, omega_skew);
    } else {
        fprintf(stderr, "Error: unknown coordinate system\n");
        exit_job(EXIT_FAILURE);
    }
}
void Qdot(double dQRdt[DIM * DIM], const double QR[DIM * DIM], const double omega[DIM], const COORD_SYSTEM &coord) {
    double omega_skew[DIM * DIM];
    skew(omega_skew, omega);
    if (coord == SPACE_FRAME) {
        M_prod(dQRdt, omega_skew, QR);
    } else if (coord == BODY_FRAME) {
        M_prod(dQRdt, QR, omega_skew);
    } else {
        fprintf(stderr, "Error: unknown coordinate system\n");
        exit_job(EXIT_FAILURE);
    }
}

// time derivative of angular velocity
/*void wdot(double dwdt[DIM],
          const double w[DIM],
          const double tau[DIM],
          const double I[DIM][DIM]){

  double dmy[DIM];
  double Iinv[DIM][DIM];
  M_inv(Iinv, I);
  M_v_prod(dmy, I, w);
  v_cross(dmy, w);
  v_add(dmy, tau);
  M_v_prod(dwdt, Iinv, dmy);
}
inline void wdot(double dwdt[DIM],
                 const double tau[DIM],
                 const double I[DIM][DIM]){
  double w[DIM];
  v_copy(w, dwdt);
  wdot(dwdt, w, tau, I);
}
*/

// Rotation operator using rodrigues' formula
/*void rodrigues_rotation_formula(double T[DIM][DIM],
                                const double phi,
                                const double n[DIM]){
  double Id[DIM][DIM] = {{1.0, 0.0, 0.0},
                         {0.0, 1.0, 0.0},
                         {0.0, 0.0, 1.0}};
  double V[DIM][DIM];

  double cs = 1.0 - cos(phi);
  double sn = sin(phi);
  skew(V, n);
  for(int i = 0; i < DIM; i++){
    for(int j = 0; j < DIM; j++){
      T[i][j] = Id[i][j] + sn*V[i][j] +
        cs * (n[i]*n[j] - Id[i][j]);
    }
  }
}
*/
//////////////////////////////////////////////////
//////////////////////////////////////////////////
/// TEST ROUTINES
//////////////////////////////////////////////////

/*void propagate_wq_RK4(quaternion &q,
                      double omega[DIM],
                      const double I[DIM][DIM],
                      const double &dt){
  quaternion kq1, kq2, kq3, kq4, dmy_q;
  double kw1[DIM], kw2[DIM], kw3[DIM], kw4[DIM], dmy_w[DIM];
  double tau[DIM] = {0.0, 0.0, 0.0};

  qtn_init(dmy_q, q);
  v_copy(dmy_w, omega);
  qdot(kq1, dmy_q, dmy_w, BODY_FRAME);
  wdot(kw1, dmy_w, tau, I);
  qtn_scale(kq1, dt);
  v_scale(kw1, dt);

  qtn_add(dmy_q, q, kq1, 0.5);
  v_add(dmy_w, omega, kw1, 0.5);
  qdot(kq2, dmy_q, dmy_w, BODY_FRAME);
  wdot(kw2, dmy_w, tau, I);
  qtn_scale(kq2, dt);
  v_scale(kw2, dt);

  qtn_add(dmy_q, q, kq2, 0.5);
  v_add(dmy_w, omega, kw2, 0.5);
  qdot(kq3, dmy_q, dmy_w, BODY_FRAME);
  wdot(kw3, dmy_w, tau, I);
  qtn_scale(kq3, dt);
  v_scale(kw3, dt);

  qtn_add(dmy_q, q, kq3);
  v_add(dmy_w, omega, kw3);
  qdot(kq4, dmy_q, dmy_w, BODY_FRAME);
  wdot(kw4, dmy_w, tau, I);
  qtn_scale(kq4, dt);
  v_scale(kw4, dt);

  qtn_add(dmy_q, kq1, kq2, 2.0);
  qtn_add(dmy_q, kq3, 2.0);
  qtn_add(dmy_q, kq4);
  qtn_add(q, dmy_q, 1.0/6.0);
  qtn_normalize(q);

  v_add(dmy_w, kw1, kw2, 2.0);
  v_add(dmy_w, kw3, 2.0);
  v_add(dmy_w, kw4);
  v_add(omega, dmy_w, 1.0/6.0);
}
*/
/*void propagate_w_RK4_Q_Euler(double QR[DIM][DIM],
                             double omega[DIM],
                             const double I[DIM][DIM],
                             const double &dt){
  double rot_angle;
  double rot_vector[DIM];
  double rot_operator[DIM][DIM];
  double dmy_QR[DIM][DIM];
  double kw1[DIM], kw2[DIM], kw3[DIM], kw4[DIM], dmy_w[DIM];
  double tau[DIM] = {0.0, 0.0, 0.0};

  // orientation update
  v_copy(rot_vector, omega);
  M_copy(dmy_QR, QR);


  rot_angle = v_norm(rot_vector);
  v_scale(rot_vector, 1.0/rot_angle);
  rot_angle *= dt;


  rodrigues_rotation_formula(rot_operator, rot_angle, rot_vector);
  M_prod(QR, dmy_QR, rot_operator);


  // omega update
  v_copy(dmy_w, omega);
  wdot(kw1, dmy_w, tau, I);
  v_scale(kw1, dt);


  v_add(dmy_w, omega, kw1, 0.5);
  wdot(kw2, dmy_w, tau, I);
  v_scale(kw2, dt);


  v_add(dmy_w, omega, kw2, 0.5);
  wdot(kw3, dmy_w, tau, I);
  v_scale(kw3, dt);


  v_add(dmy_w, omega, kw3);
  wdot(kw4, dmy_w, tau, I);
  v_scale(kw4, dt);


  v_add(dmy_w, kw1, kw2, 2.0);
  v_add(dmy_w, kw3, 2.0);
  v_add(dmy_w, kw4);
  v_add(omega, dmy_w, 1.0/6.0);

}
*/
