#include "Lift.h"

/*!
 * \brief Constructor for Lift
 *
 * Initializes liftMotor to the Lifter port
 */
Lift::Lift() : liftMotor(Ports::Lifter::Motor)
{

}

/*!
 * \brief Activates the lifter at the specified speed
 *
 * \param[in] speed The speed that the lifter will climb at
 */
void Lift::raise(float speed)
{
	liftMotor.Set(speed * Speeds::Lift::Lift);
}

/*!
 * \brief Activates the lifter in reverse
 */
void Lift::lower()
{
	liftMotor.Set(Speeds::Lift::Lower);
}

/*!
 * \brief Stops the lifter
 */
void Lift::stop()
{
	liftMotor.Set(0);
}

